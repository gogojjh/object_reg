#include "object_reg/DetectMarker.h"

namespace objectR 
{	
	double tic()
	{
		struct timeval t;
		gettimeofday(&t, nullptr);
		return ((double)t.tv_sec + ((double)t.tv_usec) / 1000000);
	}
	
	/**
	* Normalize angle to be within the interval [-pi,pi].
	*/
	inline double standardRad(double t)
 	{
		if (t >= 0.)
		{
			t = fmod(t+PI, 2*PI) - PI;
		} else 
		{
			t = fmod(t-PI, -2*PI) + PI;
		}
		return t;
	}
	
	void wRo_to_euler(const Eigen::Matrix3d &wRo, double &yaw, double &pitch, double &roll)
	{
		yaw =standardRad(atan2(wRo(1,0), wRo(0,0)));
		double c = cos(yaw);
		double s = sin(yaw);
		pitch = standardRad(atan2(-wRo(2,0), wRo(0,0)*c + wRo(1,0)*s));
		roll  = standardRad(atan2(wRo(0,2)*s - wRo(1,2)*c, -wRo(0,1)*s + wRo(1,1)*c));
	}
	
	DetectMarker::DetectMarker(Mat &im): img_(im), f_(false), objectCenter_(Point2f(0,0))
	{
		cout << "DetectMarker ..." << endl;
		FindObject();
	}
	
	void DetectMarker::FindObject()
	{
		cout << "DetectMarker::FindObject ..." << endl;
		Marker *marker = new Marker(img_);
		marker->ProcessImage(img_, markerCenters_, translations_, eulers_);
		if (markerCenters_.size() != 0) 
		{
		    for (int i=0; i<markerCenters_.size(); i++)
		    {
			cout << "X: " << markerCenters_[i].x << " Y: " << markerCenters_[i].y << endl;
			objectCenter_.x += markerCenters_[i].x / markerCenters_.size();
			objectCenter_.y += markerCenters_[i].y / markerCenters_.size();
		    }
		    f_ = true;
		}
	}

	void DetectMarker::PrintResult()
	{
	    
	}
	
//********************Marker

	Marker::Marker(const Mat im): tagDetector_(nullptr), tagCodes_(AprilTags::tagCodes16h5), timing(true), tagSize_(setting_tagSize),
		    fx_(setting_fx), fy_(setting_fy), cx_(setting_cx), cy_(setting_cy), width_(im.cols), height_(im.rows)
	{
		tagDetector_ = new AprilTags::TagDetector( tagCodes_ );
	} 
	
	void Marker::ProcessImage(Mat &img, vector<Point2f> &markerCenter, 
				  vector<Eigen::Vector3d> &translations, vector<vector<double>> &eulers)
	{
		Mat img_gray = img.clone();
		if(img_gray.channels()==3)
			cvtColor(img_gray,img_gray,CV_RGB2GRAY);
		if(img_gray.channels()==4)
			cvtColor(img_gray,img_gray,CV_BGRA2GRAY);
		
		double t0;
		if (timing)
			t0 = tic();
		
		vector<AprilTags::TagDetection> detections = tagDetector_->extractTags(img_gray);
		if (timing)
		{
			double dt = tic() - t0;
			cout << "Extracting tags took " << dt << " seconds." << endl;
		}
		
		cout << "TagDetector::extractTags Finish..." << endl;
		for (int i=0; i<detections.size(); i++)
		{
			detections[i].draw(img);
			
			Point2f center(detections[i].cxy.first, detections[i].cxy.second);
			markerCenter.push_back(center);
			
			Eigen::Vector3d translation;
			Eigen::Matrix3d rotation;
			detections[i].getRelativeTranslationRotation(tagSize_, fx_, fy_, cx_, cy_, translation, rotation);
			translations.push_back(translation);
			
			Eigen::Matrix3d F;
			F <<
				1, 0, 0,
				0, -1, 0,
				0, 0, 1;
			Eigen::Matrix3d fixed_rot = F*rotation;
			double yaw, pitch, roll;
			wRo_to_euler(fixed_rot, yaw, pitch, roll);
			vector<double> e;
			e.push_back(yaw);
			e.push_back(pitch);
			e.push_back(roll);
			eulers.push_back(e);		
		}		
	}
}


























