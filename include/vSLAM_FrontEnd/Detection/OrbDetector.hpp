#ifndef FRONTEND_ORB_DETECTOR_TYPE_HPP
#define FRONTEND_ORB_DETECTOR_TYPE_HPP

#include "vSLAM_FrontEnd/Detection/DetectorSettings.hpp"


namespace stereo
{

class OrbDetector : public DetectorSettings {
	protected:
		struct settings_
		{
			int nMax_;
			float scaleFactor_;
			int nlevels_;
			int edgeThresh_;
			int firstLevel_;
			int WTA_K_;
			int score_type_;
			int patchSize_;
		};
		cv::Ptr<cv::ORB> detector_;//=new cv::SIFT(0,10,0.03);
		settings_ startConfig_;
		settings_ currentConfig_;	
		void updateSettings(settings_ &n);
    public:
		OrbDetector(int nMax,float scaleFactor,int nlevels,int edgeThresh,int firstLevel,int WTA_K,
								 int score_type,int patchSize);
		void resetDefaults() override;
		void adjustSettings() override;
		std::string getName() override;
		void detect(cv::Mat image,std::vector<cv::KeyPoint> &output) override;
};
	
}

#endif
 
 
 
