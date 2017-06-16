#include "object_reg/DetectBlueCircle.h"

namespace objectR 
{
	DetectBlueCircle::DetectBlueCircle(const Mat im): img_(im.clone()), f_(false), objectCenter_(Point2f(0,0))
	{
		FindObject();
	}    
    
	void DetectBlueCircle::FindObject()
	{
		Mat imGray = img_.clone();
		if(imGray.channels()==3)
		    cvtColor(imGray,imGray,CV_RGB2GRAY);
		if(imGray.channels()==4)
		    cvtColor(imGray,imGray,CV_BGRA2GRAY);
		GaussianBlur(imGray, imGray, Size(9, 9), 2, 2 );
		vector<Vec3f> circles;
		HoughCircles(imGray, circles, CV_HOUGH_GRADIENT, 1, imGray.rows/8, 200, 100, imGray.rows/10, 0);
		if (circles.size() > 0)
		{
		    objectCenter_ = Point2f(cvRound(circles[0][0]), cvRound(circles[0][1]));
		    f_ = true;
		}    
	   
// 		Mat imgSingle = imageChannelSplit(img_, BGR_R);
// 		vector<vector<Point> > contours;
// 		vector<Vec4i> hierarchy;
// 		float maxContourArea = 0;
// 		int maxContourArea_index = 0;
// 
// 		//Mat element = getStructuringElement(MORPH_RECT,Size(27,27));
// 		//morphologyEx(imgSingle, imgSingle,MORPH_DILATE,element);
// 		//imshow("2_img_dilate",imgSingle);
// 		
// 		//medianBlur(imgSingle,imgSingle,5);
// 
// 		//element = getStructuringElement(MORPH_RECT,Size(22,22));
// 		//morphologyEx(imgSingle,imgSingle,MORPH_ERODE,element);
// 		//imshow("3_img_close",imgSingle);
// 
// 		//Mat element = getStructuringElement(MORPH_RECT,Size(5,5));
// 		//morphologyEx(imgSingle, imgSingle, MORPH_CLOSE, element);
// 		//imshow("img_single_close",imgSingle);
// 
// 		//threshold(imgSingle, imgSingle, 60 , 255, THRESH_BINARY_INV );
// 		//imshow("img_single_threshold",imgSingle);
// 
// 		findContours(imgSingle, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
// 		drawContours(imgSingle, contours, -1, Scalar(255), 2);
// 		imshow("contours",imgSingle);
// 
// 		// Find the max area of contours
// 		for (int i=0;i<contours.size();i++)
// 		{
// 			int contourSize = contourArea(contours[i]);
// 			//cout<<"area"<<contourSize<<endl;
// 			if (contourSize < img_.cols * img_.rows * 0.066) continue;
// 			if (contourSize > maxContourArea)
// 			{
// 				maxContourArea = contourSize;
// 				maxContourArea_index = i;
// 			}
// 		}
// 		// cout << "Max area: " << maxContourArea << endl;
// 
// 		RotatedRect box;
// 		if(maxContourArea != 0)
// 		{
// 			box = minAreaRect(contours[maxContourArea_index]);
// 			f_ = true;
// 		}
// 		
// 		Point2f vertex[4];
// 		box.points(vertex);
// 
// 		for(int i=0;i<4;i++)    
// 		{
// 			line(img_, vertex[i], vertex[(i+1)%4], Scalar(0,0,255), 5);    //draw the rectangle of the box
// 		}
// 		objectCenter_.x = (vertex[1].x+vertex[3].x)/2;
// 		objectCenter_.y = (vertex[1].y+vertex[3].y)/2;
// 		Point2f center(img_.cols/2, img_.rows/2);
// 		circle(img_, objectCenter_, 3, Scalar(0,0,255), 2, 8, 0);
	}
	
	void DetectBlueCircle::PrintResult()
	{
		cout << "If Find object = " << f_ << endl
			<< "x = " << objectCenter_.x << " y = " << objectCenter_.y << endl;
	}

}


