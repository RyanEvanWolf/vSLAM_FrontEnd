 
#include "vSLAM_FrontEnd/Detection/AkazeDetector.hpp"


namespace stereo {

AkazeDetector::AkazeDetector(AKAZEOptions in)
{
	startConfig_=in;
	currentConfig_=in;
	updateSettings(in);
	detector_= new libAKAZE::AKAZE(currentConfig_);
}


void AkazeDetector::detect(cv::Mat image, std::vector< cv::KeyPoint >& output)
{
	cv::Mat floatImage;
	image.convertTo(floatImage,CV_32F,1.0/255.0,0);
	detector_->Create_Nonlinear_Scale_Space(floatImage);
	detector_->Feature_Detection(output);
}

void AkazeDetector::extract(cv::Mat image, std::vector< cv::KeyPoint >& in, cv::Mat& out)
{

}


void AkazeDetector::resetDefaults()
{
	updateSettings(startConfig_);
	//currentThreshold_=threshold_;
}


void AkazeDetector::adjustSettings()
{

}

void AkazeDetector::updateSettings(AKAZEOptions &n)
{
	detector_.release();
	detector_=new libAKAZE::AKAZE(n);
}


std::string AkazeDetector::getName()
{
	std::stringstream outName;
	outName<<"KAZE_"<<currentConfig_.soffset<<"_"<<currentConfig_.omax<<"_";
	outName<<currentConfig_.nsublevels<<"_"<<currentConfig_.dthreshold<<"_";
	outName<<currentConfig_.min_dthreshold<<"_";
	outName<<currentConfig_.diffusivity<<"_"<<currentConfig_.sderivatives<<"_";
	outName<<currentConfig_.kcontrast<<"_"<<currentConfig_.kcontrast_percentile<<"_";
	outName<<currentConfig_.kcontrast_nbins;
	return outName.str();
}



	
	
	
}
