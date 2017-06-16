#include "object_reg/DetectYellowLine.h"

namespace objectR 
{
	DetectYellowLine::DetectYellowLine(const Mat im): img_(im.clone()), f_(false), objectCenter_(Point2f(0,0))
	{
		FindObject();
		erodeElement_ = getStructuringElement(MORPH_RECT, Size(5, 5));
		dilateElement_ = getStructuringElement(MORPH_RECT, Size(5, 5));		
	}    
    
	void DetectYellowLine::FindObject()
	{
		Mat grayFrame, hsvFram;
		vector<Mat> hsvSplit;

		cvtColor(img_, hsvFram, COLOR_BGR2HSV);  

		split(hsvFram, hsvSplit);                    
		equalizeHist(hsvSplit[2], hsvSplit[2]);
		merge(hsvSplit, hsvFram);

		// threshold
		int iLowH = 26, iHighH = 35;
		int iLowS = 38, iHighS = 255;
		int iLowV = 46, iHighV = 255;

		Mat imgThreshold, imgtreated;
		inRange(hsvFram, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThreshold);


		// open operation
		Mat element = getStructuringElement(MORPH_RECT, Size(7, 7));
		morphologyEx(imgThreshold, imgtreated, MORPH_OPEN, element);

		// close operation 
		morphologyEx(imgtreated, imgtreated, MORPH_CLOSE, element);

		const int rowNumber = imgtreated.rows;
		const int colNumber = imgtreated.cols; 

		double x[4] = { 20, rowNumber / 3, rowNumber*2  / 3, rowNumber-30 };
		double y[4] = { 0 };
		double k[4] = { 0 };

		double value;
		for (int i = 0; i < 4; i++)
		{
			uchar* point = imgtreated.ptr<uchar>(x[i]);
			for (int j = 0; j < colNumber; j++)
			{
				value = (double)point[j];
				if (value > 200)
				{
					y[i] = j;
					break;
				}
			}
		}

		if (y[3] != 0)
		{
			k[0] = ((x[3] - x[0]) / (y[0] - y[3]));
			k[1] = ((x[3] - x[1]) / (y[1] - y[3]));
			k[2] = ((x[3] - x[2]) / (y[2] - y[3]));
			k[3] = ((x[2] - x[1]) / (y[1] - y[2]));
		}
		else
		{
			k[0] = ((x[2] - x[0]) / (y[0] - y[2]));
			k[1] = ((x[2] - x[1]) / (y[1] - y[2]));
			k[2] = ((x[1] - x[2]) / (y[2] - y[1]));
		}
		cout << "y[]  : " << endl;
		for (int i = 0; i < 4; i++)
		  cout << y[i]/colNumber << "     " << endl;                 

 		for (int i = 0; i < 4; i++) 			
			cout << k[i] << "     " << endl;

		// line spilt

		cvtColor(img_, grayFrame, CV_RGB2GRAY);     
		Mat smoothFrame,dstImage;
		GaussianBlur(grayFrame, smoothFrame, Size(5, 5), 0, 0);    

		dilate(smoothFrame, smoothFrame, dilateElement_);
		erode(smoothFrame, smoothFrame, erodeElement_);

		// canny detector
		Canny(smoothFrame, smoothFrame, 150, 150);
		cvtColor(smoothFrame, dstImage, CV_GRAY2BGR);
		vector<Vec4i> lines;
		HoughLinesP(smoothFrame, lines, 1, CV_PI / 180, 40, 0, 10);
		vector<float> theta;
		for (size_t i = 0; i < lines.size(); i++)
		{
			theta.push_back(cvFastArctan(abs(lines[i][3] - lines[i][1]), abs(lines[i][2] - lines[i][0])));
		}
		
		for (size_t j = 0; j < lines.size(); j++)
		{
			for (size_t k = j + 1; k < lines.size(); k++)
			{
				if (abs(theta[j] - theta[k]) < 2)
				{
					line(dstImage, Point(lines[j][0], lines[j][1]),
						Point(lines[j][2], lines[j][3]), Scalar(186, 88, 255), 1, CV_AA);
					line(dstImage, Point(lines[k][0], lines[k][1]),
						Point(lines[k][2], lines[k][3]), Scalar(186, 88, 255), 1, CV_AA);
				}

			}
		}	    
	}
	
	void DetectYellowLine::PrintResult()
	{
		cout << "If Find object = " << f_ << endl
			<< "x = " << objectCenter_.x << " y = " << objectCenter_.y << endl;
	}

}


