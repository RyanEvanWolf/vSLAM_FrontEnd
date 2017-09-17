#ifndef FRONTEND_SIFT_DETECTOR_TYPE_HPP
#define FRONTEND_SIFT_DETECTOR_TYPE_HPP

#include "vSLAM_FrontEnd/Detection/DetectorSettings.hpp"


namespace stereo
{

class SiftDetector : public DetectorSettings {
	protected:
		struct settings_
		{
			int nBest_;
			int octave_;
			double contrastThresh_;
			double edgeThresh_;
			double gaussSigma_;
		};
		cv::Ptr<cv::SIFT> detector_;//=new cv::SIFT(0,10,0.03);
		settings_ startConfig_;
		settings_ currentConfig_;	
		void updateSettings(settings_ &n);
    public:
		SiftDetector();
		SiftDetector(int nfeatures, int nOctaveLayers, double contrastThreshold, double edgeThreshold, double sigma);
		void resetDefaults() override;
		void adjustSettings() override;
		std::string getName() override;
		void detect(cv::Mat image,std::vector<cv::KeyPoint> &output) override;
		void extract(cv::Mat image,std::vector<cv::KeyPoint> &in,cv::Mat &out) override;
};
	
}

#endif
 
 
