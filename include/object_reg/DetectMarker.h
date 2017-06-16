#ifndef DETECTMARKER_H
#define DETECTMARKER_H

#include "object_reg/setting.h"
#include "object_reg/DetectImage.h"

#include <sys/time.h>
#include <libv4l2.h>
#include <fcntl.h>
#include <errno.h>

#include <unistd.h>

#include "AprilTags/TagDetector.h"
#include "AprilTags/TagDetection.h"
#include "AprilTags/Tag16h5.h"
#include "AprilTags/Tag25h7.h"
#include "AprilTags/Tag25h9.h"
#include "AprilTags/Tag36h9.h"
#include "AprilTags/Tag36h11.h"

using namespace cv;
using namespace std;

namespace objectR
{
	class DetectMarker : public DetectImage 
	{
	public:
		DetectMarker(Mat &im);
		virtual void FindObject();
		virtual void PrintResult();

		Mat img_;
		bool f_;
		Point2f objectCenter_;
		vector<Point2f> markerCenters_;
		vector<Eigen::Vector3d> translations_;
		vector<vector<double>> eulers_;
	};	
	
	class Marker 
	{
	public:
		AprilTags::TagDetector *tagDetector_; 
		AprilTags::TagCodes tagCodes_; 
		
		int width_;
		int height_;
		double tagSize_;
		double fx_, fy_, cx_, cy_; 
		bool timing;
		Point2f objectCerter_;
		
		Marker(const Mat im);
		void ProcessImage(Mat &img, vector<Point2f> &markerCenter, 
				  vector<Eigen::Vector3d> &translations, vector<vector<double>> &eulers);
	};
		

}

#endif // DETECTMARKER_H





















