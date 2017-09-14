 
#ifndef FRONTEND_FREAK_DETECTOR_TYPE_HPP
#define FRONTEND_FREAK_DETECTOR_TYPE_HPP

#include "vSLAM_FrontEnd/Detection/DetectorSettings.hpp"


namespace stereo
{

class FreakDetector : public DetectorSettings {
	protected:
		struct settings_
		{
			bool orientNormal_;
			bool scaleNormal_;
			float patternScale_;
			int octaves_;
			//check
		};
		cv::Ptr<cv::FREAK> detector_;//=new cv::SIFT(0,10,0.03);
		settings_ startConfig_;
		settings_ currentConfig_;	
		void updateSettings(settings_ &n);
    public:
		FreakDetector(bool orient,bool scale, float pattern,int oct);
		void resetDefaults() override;
		void adjustSettings() override;
		std::string getName() override;
		void detect(cv::Mat image,std::vector<cv::KeyPoint> &output) override;
};
	
}

#endif
 
 
 
