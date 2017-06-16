#ifndef IMAGEREADER_H
#define IMAGEREADER_H

#include <string>

#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include "object_reg/pictureflag.h"
#include "object_reg/pictureresult.h"

#include "object_reg/setting.h"
#include "object_reg/DetectBlueCircle.h"
#include "object_reg/DetectMarker.h"
#include "object_reg/DetectYellowLine.h"

using namespace cv;
using namespace std;


namespace objectR
{	
	class ImageReader
	{
	public:
		ImageReader()
		{
		}
			
		void TrackMonoImage(string key)
		{
		    resize(imgOriginal, imgProcessed, Size(imgOriginal.cols*image_sizeRatio, imgOriginal.rows*image_sizeRatio));
// 		    imshow("Track_Processed",imgProcessed);
// 		    waitKey(1000);
		    if (key == "marker")
		    {
			detectMarker = new DetectMarker(imgProcessed);
			objectFind = detectMarker->f_;
			objectCenter = detectMarker->objectCenter_;
			//detectMarker->PrintResult();
			circle(imgProcessed, objectCenter, 6, Scalar(0,0,255), 2);
			cout << "DetectMarker Result: " << objectFind << objectCenter << endl;
		    }
		    else if (key == "circle")
		    {
			blueCircle = new DetectBlueCircle(imgProcessed);
			objectFind = blueCircle->f_;
			objectCenter = blueCircle->objectCenter_;
			//blueCircle->PrintResult();
			circle(imgProcessed, objectCenter, 6, Scalar(0,0,255), 2);
			cout << "DetectCircle Result: " << objectFind << objectCenter << endl;
		    }
		    else if (key == "line")
		    {
			yellowLine = new DetectYellowLine(imgProcessed);
			objectFind = yellowLine->f_;
			objectCenter = yellowLine->objectCenter_;
			//yellowLine->PrintResult();
			circle(imgProcessed, objectCenter, 6, Scalar(0,0,255), 2);
			cout << "DetectCircle Result: " << objectFind << objectCenter << endl;
		    }		    
		}
			
		void GetImageSize(int &w, int &h)
		{
		    w = imgOriginal.cols;
		    h = imgOriginal.rows;
		}
		
		void PictureFlagCallBack(const object_reg::pictureflag &msg)
		{
		    this->target = msg.target;
		}

		void GrabImageCallBack(const sensor_msgs::ImageConstPtr &msg)
		{
// 		    ROS_INFO("Enter GrabImageCallBack");
		    cv_bridge::CvImageConstPtr cv_ptr;
		    try
		    {
			cv_ptr = cv_bridge::toCvShare(msg);
		    }
		    catch (cv_bridge::Exception &e)
		    {
			ROS_ERROR("cv_bridge exception: %s", e.what());
			return;
		    }
 		    imgOriginal = cv_ptr->image.clone();
		}
		
		DetectBlueCircle *blueCircle;
		DetectMarker *detectMarker;
		DetectYellowLine *yellowLine;
		
		bool objectFind;
		Point2f objectCenter;
		Mat imgOriginal, imgProcessed;
		int target;
		
	private:
	    
	};
}
#endif // IMAGEREADER_H
