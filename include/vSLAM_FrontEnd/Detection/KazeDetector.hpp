 
#ifndef FRONTEND_KAZE_DETECTOR_TYPE_HPP
#define FRONTEND_KAZE_DETECTOR_TYPE_HPP

#include "vSLAM_FrontEnd/Detection/DetectorSettings.hpp"
#include <kaze/KAZE.h>

namespace stereo
{

class KazeDetector : public DetectorSettings {
	protected:
		KAZEOptions startConfig_;
		KAZEOptions currentConfig_;
		cv::Ptr<libKAZE::KAZE> detector_;
		void updateSettings(KAZEOptions &n);
    public:
		KazeDetector(KAZEOptions in);
		void resetDefaults() override;
		void adjustSettings() override;
		std::string getName() override;
		void detect(cv::Mat image,std::vector<cv::KeyPoint> &output) override;
		void extract(cv::Mat image,std::vector<cv::KeyPoint> &in,cv::Mat &out) override;
};
	
}

#endif
 
 
