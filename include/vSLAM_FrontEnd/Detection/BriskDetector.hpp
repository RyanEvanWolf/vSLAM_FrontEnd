#ifndef FRONTEND_SIFT_DETECTOR_TYPE_HPP
#define FRONTEND_BRISK_DETECTOR_TYPE_HPP

#include "vSLAM_FrontEnd/Detection/DetectorSettings.hpp"


namespace stereo
{

class BriskDetector : public DetectorSettings {
	protected:
		struct settings_
		{
			int thresh_;
			int octave_;
			float pattern_;
		};
		cv::Ptr<cv::BRISK> detector_;//=new cv::SIFT(0,10,0.03);
		settings_ startConfig_;
		settings_ currentConfig_;	
		void updateSettings(settings_ &n);
    public:
		BriskDetector(int thresh, int oct,float pattern);
		void resetDefaults() override;
		void adjustSettings() override;
		std::string getName() override;
		void detect(cv::Mat image,std::vector<cv::KeyPoint> &output) override;
		oid extract(cv::Mat image,std::vector<cv::KeyPoint> &in,cv::Mat &out) override;
};
	
}

#endif
 
 
 
