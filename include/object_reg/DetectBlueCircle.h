#ifndef DETECTBLUECIRCLE_H
#define DETECTBLUECIRCLE_H

#include "object_reg/setting.h"
#include "object_reg/DetectImage.h"

using namespace cv;
using namespace std;

namespace objectR
{
	class DetectBlueCircle : public DetectImage
	{
	public:
		DetectBlueCircle(const Mat img);
		
		virtual void FindObject();
		virtual void PrintResult();
		
		Mat img_;
		bool f_;
		Point2f objectCenter_;
	};	
}

#endif // DETECTBLUECIRCLE_H
