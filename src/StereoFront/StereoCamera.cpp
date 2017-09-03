#include "vSLAM_FrontEnd/StereoFront/StereoCamera.hpp"
namespace stereo
{
StereoCamera::StereoCamera(DetectorSettings* left,DetectorSettings* right)
{
	leftDet=left;
	rightDet=right;
}

void StereoCamera::extractStereoFrame(cv::Mat leftIn, cv::Mat rightIn, StereoFrame& outFrame)
{

}




	
	
}
