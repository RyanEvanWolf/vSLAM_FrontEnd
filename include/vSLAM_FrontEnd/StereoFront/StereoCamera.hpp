#ifndef STEREO_CAMERA_HEADERS_HPP
#define STEREO_CAMERA_HEADERS_HPP

#include "Structures/vSLAM/StereoFrame.hpp"
#include "Structures/CameraInfo/StereoRect.hpp"
#include "vSLAM_FrontEnd/Detection/DetectorSettings.hpp"

namespace stereo 
{
	
class StereoCamera 
{
	private:
		DetectorSettings *leftDet,*rightDet;
	public:
		StereoCamera(DetectorSettings* left,DetectorSettings* right);
		void extractStereoFrame(cv::Mat leftIn,cv::Mat rightIn,StereoFrame &outFrame);
};
	
	
}

#endif
