#include "object_reg/DetectBlueCircle.h"

namespace objectR 
{
	DetectBlueCircle::DetectBlueCircle(const Mat im): img_(im.clone()), f_(false), objectCenter_(Point2f(0,0))
	{
		FindObject();
	}    
    
	void DetectBlueCircle::FindObject()
	{
// 		Mat imGray = img_.clone();
// 		if(imGray.channels()==3)
// 		    cvtColor(imGray,imGray,CV_RGB2GRAY);
// 		if(imGray.channels()==4)
// 		    cvtColor(imGray,imGray,CV_BGRA2GRAY);
// 		GaussianBlur(imGray, imGray, Size(9, 9), 2, 2 );
// 		vector<Vec3f> circles;
// 		HoughCircles(imGray, circles, CV_HOUGH_GRADIENT, 1, imGray.rows/8, 200, 100, imGray.rows/10, 0);
// 		if (circles.size() > 0)
// 		{
// 		    objectCenter_ = Point2f(cvRound(circles[0][0]), cvRound(circles[0][1]));
// 		    f_ = true;
// 		}    
	   
		
// 		string path = "/home/chiao/desktop/20170330212802.avi";
// 		VideoCapture capture(path);
// 
// 		if(!capture.isOpened())
// 			cout<<"error"<<endl;
		
		int filted = 0;     //number of circles that passed
		int not_det = 0;    //number of frames that no circles
		//parameters
		float ratio = 0.3; //resize ratio
		int min_radius = 100 * ratio;
		int max_radius = 350 * ratio;
		int hsv_thrs = 15;
		int dist_thrs = 90 * ratio; //the dist of current circle and the last one
		int pt_range = 8 * ratio;
		int pts_thrs = (2 * pt_range + 1) * (2 * pt_range + 1) * 0.6;   //60%
		int value_thrs = 180;
		Point2i last_circle = Point2i(0, 0);

		Mat frame;
		int frame_num = 0;
// 		while(capture.read(frame))
// 		{
		frame_num++;
// 		stringstream ss;
// 		ss << "/home/chiao/documents/catkin_ws/src/object_reg/image/circle/" << frame_num << ".png";
// 		imwrite(ss.str(), frame);
		frame = img_.clone();
// 		resize(frame, frame, Size(), ratio, ratio);

		Mat hsv;
		cvtColor(frame, hsv, COLOR_BGR2HSV);

		//define range of blue & red in HSV
		int lower_blue = 120 - hsv_thrs;
		int upper_blue = 120 + hsv_thrs;

		int lower_red1 = 0;
		int upper_red1 = hsv_thrs;

		int lower_red2 = 180 - hsv_thrs;
		int upper_red2 = 180;

		//threshold the hsv image
		Mat mask_blue, mask_red1, mask_red2;
		inRange(hsv, Scalar(lower_blue, 50, 50), Scalar(upper_blue, 255, 255), mask_blue);
		inRange(hsv, Scalar(lower_red1, 50, 50), Scalar(upper_red1, 255, 255), mask_red1);
		inRange(hsv, Scalar(lower_red2, 50, 50), Scalar(upper_red2, 255, 255), mask_red2);

		Mat element = getStructuringElement(MORPH_RECT, Size(3, 3));
		morphologyEx(mask_blue, mask_blue, MORPH_CLOSE, element);
		morphologyEx(mask_red1, mask_red1, MORPH_CLOSE, element);
		morphologyEx(mask_red2, mask_red2, MORPH_CLOSE, element);

		Mat mask = mask_blue + mask_red1 + mask_red2;

		Mat blur;
		GaussianBlur(mask, blur, Size(5, 5), 2);

		//use Hough transform to detect circle
		vector<Vec3f> circles;
		HoughCircles(blur, circles, CV_HOUGH_GRADIENT, 1,
			1,    //the min distance between two circles
			150,           //gradient value used to handle edge detection
			50,            //the smaller the threshold is, the more circles will be detected
			min_radius,
			max_radius);

		//select the best one
		bool flag = false;

		if(circles.size() )
		{
			int best_index = -1;
			int max_cnt = 0;
			float min_dist = 2000.0;
			for(size_t i = 0; i < circles.size(); i++)
			{
				int cnt = 0;
				Vec3f cir = circles[i];
				for(int x = -pt_range; x <= pt_range; x++)
				for(int y = -pt_range; y <= pt_range; y++)
				{
					if((int)mask.at<uchar>(cir[1] + x, cir[0] + y) > value_thrs)
					cnt++;
				}

				if(cnt < pts_thrs)  //pass the point not in the bright center
					continue;

				if(last_circle.x == 0 && last_circle.y == 0)
				{
					if(cnt > max_cnt)
					{
						max_cnt = cnt;
						best_index = i; //accept the point in the bright center
					}
				}
				else
				{
					float dist = sqrt((cir[0] - last_circle.x) * (cir[0] - last_circle.x) +
						(cir[1] - last_circle.y) * (cir[1] - last_circle.y));
					if(dist > dist_thrs)
						continue;
					else if(dist < min_dist)    //accept the point closest to the last one
					{
						min_dist = dist;
						best_index = i;
					}
				}
			}

			if(best_index != -1)
			{
				flag = true;
				Vec3f best_circle = circles[best_index];
				circle(frame, Point2f(best_circle[0], best_circle[1]), best_circle[2], Scalar(0, 255, 0), 2);
				circle(frame, Point2f(best_circle[0], best_circle[1]), 3, Scalar(0, 0, 255), 2);
				last_circle.x = best_circle[0];
				last_circle.y = best_circle[1];
			}
		}

		if(flag == false)
		{
			last_circle.x = 0;
			last_circle.y = 0;
			//cout<<"#frame: "<<frame_num<<" size: "<<circles.size()<<endl;
			if(circles.size() == 0) not_det++;
			filted++;
		}
		
		objectCenter_ = Point2f(cvRound(last_circle.x), cvRound(last_circle.y));
		f_ = flag;    

		//visualization
// 		imshow("mask", mask);
// 		imshow("video", frame);
// 		waitKey(0);
// 		}
	}
	
	void DetectBlueCircle::PrintResult()
	{
		cout << "If Find object = " << f_ << endl
			<< "x = " << objectCenter_.x << " y = " << objectCenter_.y << endl;
	}

}


