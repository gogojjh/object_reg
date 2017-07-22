#include "object_reg/setting.h"

using namespace std;
using namespace cv;

float image_sizeRatio = 0.3;
float blueCircle_thresholdRatio = 0.066;
float marker_thresholdRatio = 0.066;

float setting_tagSize = 0.166;

float setting_fx = 600;
float setting_fy = 600;
float setting_cx = 320;
float setting_cy = 240;
	
namespace objectR 
{
	Mat imageChannelSplit(Mat img, ChannelType key)
	{
		if  (key==HSV || key==HSV_H || key==HSV_S || key==HSV_V)
		{
			Mat img_hsv, img_h, img_s, img_v;
			cvtColor(img, img_hsv, CV_BGR2HSV);
			vector< Mat > hsv_channels;
			split (img_hsv, hsv_channels);
			img_h = hsv_channels[0];
			img_s = hsv_channels[1];
			img_v = hsv_channels[2];
			switch (key)
			{
				case 0: return img_hsv;
				case 1: return img_h;
				case 2: return img_s;
				case 3: return img_v;
				default: break;
			}
		}
		
		if (key ==BGR || key ==BGR_B || key ==BGR_G || key ==BGR_R)
		{
			Mat b_img, g_img, r_img;
			vector< Mat > bgr_channels;
			split(img,bgr_channels);
			b_img = bgr_channels[0];
			g_img = bgr_channels[1];
			r_img = bgr_channels[2];
			switch ( key )
			{
				case 4: return img;
				case 5: return b_img;
				case 6: return g_img;
				case 7: return r_img;
			}
		}
		return img;
	}

	// get the position of destination
	void getPosition(const Point2f p1, const Point2f p2, Point2f &ans)
	{
		ans.y=p2.x - p1.x;
		ans.x=(-1)* (p2.y - p1.y);
	}
}


