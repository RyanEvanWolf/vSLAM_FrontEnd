#include "vSLAM_FrontEnd/StereoFront/StereoCamera.hpp"
namespace stereo
{
StereoCamera::StereoCamera(cv::Ptr<DetectorSettings> dl,cv::Ptr<DetectorSettings> dr,
					 cv::Ptr<DetectorSettings> del,cv::Ptr<DetectorSettings> der,
					 std::string stereoInputDir)
{
	ldet=dl;
	rdet=dr;
	ldesc=del;
	rdesc=der;
	
	cv::FileStorage c(stereoInputDir,cv::FileStorage::READ);
	if(!c.isOpened())
	{
		std::cerr<<"Stereo Configuration file not found\n";
	}
	c["StereoRect"]>>cameraSettings_;
	c.release();
	
	lundistort_=cv::Mat(cameraSettings_.L_fMapx_.size(),CV_8UC1);
	rundistort_=cv::Mat(cameraSettings_.R_fMapx_.size(),CV_8UC1);
	
	lroi_=lundistort_(cameraSettings_.l_ROI_);
	rroi_=rundistort_(cameraSettings_.r_ROI_);
}

void StereoCamera::extractStereoFrame(cv::Mat leftIn, cv::Mat rightIn, StereoFrame& outFrame)
{
	cv::remap(leftIn,lundistort_,cameraSettings_.L_fMapx_,cameraSettings_.L_fMapy_,cv::INTER_LINEAR);
	cv::remap(rightIn,rundistort_,cameraSettings_.R_fMapx_,cameraSettings_.R_fMapy_,cv::INTER_LINEAR);
	
	ldet->detect(lroi_,outFrame.leftFeatures_);
	rdet->detect(rroi_,outFrame.rightFeatures_);
	
	ldet->extract(lroi_,outFrame.leftFeatures_,outFrame.leftDescrip_);
	rdet->extract(rroi_,outFrame.rightFeatures_,outFrame.rightDescrip_);
}




	
	
}
