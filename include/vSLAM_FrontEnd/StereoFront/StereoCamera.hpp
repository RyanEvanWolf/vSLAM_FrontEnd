#ifndef STEREO_CAMERA_HEADERS_HPP
#define STEREO_CAMERA_HEADERS_HPP

#include "Structures/vSLAM/StereoFrame.hpp"
#include "Structures/CameraInfo/StereoRect.hpp"
#include "vSLAM_FrontEnd/Detection/DetectorSettings.hpp"
#include "Structures/vSLAM/StereoFeatures.hpp"
#include <opencv2/imgproc.hpp>
namespace stereo 
{
	
class StereoCamera 
{
	private:
		StereoRect cameraSettings_;
		cv::Ptr<DetectorSettings> ldet,rdet;
		cv::Ptr<DetectorSettings> ldesc,rdesc;
	public:
		cv::Mat lundistort_,rundistort_;
		cv::Mat lroi_,rroi_;
		StereoCamera(cv::Ptr<DetectorSettings> dl,cv::Ptr<DetectorSettings> dr,
					 cv::Ptr<DetectorSettings> del,cv::Ptr<DetectorSettings> der,
					 std::string stereoInputDir);
		void extractStereoFrame(cv::Mat leftIn,cv::Mat rightIn,StereoFrame &outFrame);
};
	
	
}

#endif
