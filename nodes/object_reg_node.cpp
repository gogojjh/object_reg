#include "ros/ros.h"
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include "object_reg/pictureflag.h"
#include "object_reg/pictureresult.h"

#include "opencv2/opencv.hpp"

#include "object_reg/setting.h"
#include "object_reg/ImageReader.h"

using namespace cv;
using namespace objectR;

object_reg::pictureresult pictureresult;

int main(int argc, char **argv)
{
        ImageReader *imagereader = new ImageReader();
	
	ros::init(argc, argv, "objectR_node");
	ros::NodeHandle nh;
	ros::Subscriber pictureFlagSub = nh.subscribe("/cv/pictureflag", 1, &ImageReader::PictureFlagCallBack, imagereader);
	image_transport::ImageTransport it(nh);
	image_transport::Subscriber cameraSub = it.subscribe("/usb_cam0/usb_cam/image_raw", 10, &ImageReader::GrabImageCallBack, imagereader);
	ros::Publisher pictureResultPub = nh.advertise<object_reg::pictureflag>("/cv/pictureresult", 10);
	ros::Rate r(30);
	ROS_INFO("Initialization ...");
	
	while (ros::ok())
	{
	    // please uncomment if you want to test this algorithm offline
// 	    imagereader->imgOriginal = imread("/home/floatsdsds/document/catkin_ws/src/object_reg/image/circle.png");	    
  	    if (!imagereader->imgOriginal.empty())
 	    {
 		ROS_INFO("%d", imagereader->target);
 		switch (imagereader->target)
// 		switch (2)
		{
		    case 0:break;
		    case 1:imagereader->TrackMonoImage("marker");
		    case 2:imagereader->TrackMonoImage("circle");
		    case 3:imagereader->TrackMonoImage("line");
		    default:;
		}
		if (imagereader->objectFind) 
		{
		    cout << imagereader->objectCenter << endl;		    
		}
		namedWindow("Original image", CV_WINDOW_NORMAL);
		imshow("Original image", imagereader->imgOriginal);
			
		namedWindow("Processed image", CV_WINDOW_NORMAL);
		imshow("Processed image", imagereader->imgProcessed);
	    }
	    waitKey(1);
	    
	    // coordinate system transformation
	    pictureresult.objectfind = imagereader->objectFind;
	    pictureresult.x = (-1)*(imagereader->objectCenter.y - imagereader->imgOriginal.rows/2);
	    pictureresult.y = imagereader->objectCenter.x - imagereader->imgOriginal.cols/2;
	    pictureResultPub.publish(pictureresult);
	    
	    ros::spinOnce();
	    r.sleep();
	}
        return 0;
}


 

