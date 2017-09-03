#ifndef FRONTEND_FAST_DETECTOR_TYPE_HPP
#define FRONTEND_FAST_DETECTOR_TYPE_HPP

#include "vSLAM_FrontEnd/Detection/DetectorSettings.hpp"


namespace stereo
{

class FastDetector : public DetectorSettings {
	protected:
		int threshold_;
		bool suppression_;
		int type_;
		int currentThreshold_;
		//adjustment terms
		
    public:
		FastDetector(int initialThreshold_,bool suppress,int patternType);
		void resetDefaults() override;
		void adjustSettings() override;
		std::string getName() override;
		void detect(cv::Mat image,std::vector<cv::KeyPoint> &output) override;
};
	
}

#endif
 
