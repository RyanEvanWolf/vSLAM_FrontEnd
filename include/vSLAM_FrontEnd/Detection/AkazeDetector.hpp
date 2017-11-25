 
#ifndef FRONTEND_AKAZE_DETECTOR_TYPE_HPP
#define FRONTEND_AKAZE_DETECTOR_TYPE_HPP

#include "vSLAM_FrontEnd/Detection/DetectorSettings.hpp"
#include <AKAZE.h>

namespace stereo
{

class AkazeDetector : public DetectorSettings {
	protected:
		AKAZEOptions startConfig_;
		AKAZEOptions currentConfig_;
		cv::Ptr<libAKAZE::AKAZE> detector_;
		void updateSettings(AKAZEOptions &n);
    public:
		AkazeDetector(AKAZEOptions in);
		void resetDefaults() override;
		void adjustSettings() override;
		std::string getName() override;
		void detect(cv::Mat image,std::vector<cv::KeyPoint> &output) override;
		void extract(cv::Mat image,std::vector<cv::KeyPoint> &in,cv::Mat &out) override;
};
	
}

#endif
 
 
