#ifndef DETECTYELLOWLINE_H
#define DETECTYELLOWLINE_H

#include "object_reg/setting.h"
#include "object_reg/DetectImage.h"

using namespace cv;
using namespace std;

namespace objectR
{
	class DetectYellowLine : public DetectImage
	{
	public:
		DetectYellowLine(const Mat img);
		
		virtual void FindObject();
		virtual void PrintResult();
		
		Mat img_;
		bool f_;
		Point2f objectCenter_;
		
		Mat erodeElement_, dilateElement_;
	};	
}

#endif // DETECTYELLOWLINE_H
