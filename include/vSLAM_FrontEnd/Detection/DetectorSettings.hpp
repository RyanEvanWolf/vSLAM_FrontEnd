#ifndef FRONTEND_DETECTOR_SETTINGS_HPP
#define FRONTEND_DETECTOR_SETTINGS_HPP

#include <opencv2/features2d.hpp>
#include <opencv2/nonfree/nonfree.hpp>
#include <iostream>

namespace stereo{


class DetectorSettings {
    public:
		virtual void detect(cv::Mat image,std::vector<cv::KeyPoint> &output)=0;
		virtual void adjustSettings()=0;
		virtual void resetDefaults()=0;
		virtual std::string getName()=0;
		virtual ~DetectorSettings(){};
};

}
	

#endif
