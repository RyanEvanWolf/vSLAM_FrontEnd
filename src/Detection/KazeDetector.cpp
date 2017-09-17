 
#include "vSLAM_FrontEnd/Detection/KazeDetector.hpp"


namespace stereo {
	
//KazeDetector::KazeDetector(int initialThreshold_,bool suppress,int patternType)
//{
	//threshold_=initialThreshold_;
	//currentThreshold_=initialThreshold_;
	//suppression_=suppress;
	//type_=patternType;
//}

KazeDetector::KazeDetector(KAZEOptions in)
{
	startConfig_=in;
	currentConfig_=in;
	updateSettings(in);
	detector_= new libKAZE::KAZE(currentConfig_);
}


void KazeDetector::detect(cv::Mat image, std::vector< cv::KeyPoint >& output)
{
	cv::Mat floatImage;
	image.convertTo(floatImage,CV_32F,1.0/255.0,0);
	detector_->Create_Nonlinear_Scale_Space(floatImage);
	
	detector_->Feature_Detection(output);
}

void KazeDetector::resetDefaults()
{
	updateSettings(startConfig_);
	//currentThreshold_=threshold_;
}


void KazeDetector::adjustSettings()
{

}

void KazeDetector::updateSettings(KAZEOptions &n)
{
	detector_.release();
	detector_=new libKAZE::KAZE(n);
}


std::string KazeDetector::getName()
{
	std::stringstream outName;
	outName<<"KAZE_"<<currentConfig_.soffset<<"_"<<currentConfig_.omax<<"_";
	outName<<currentConfig_.nsublevels<<"_"<<currentConfig_.dthreshold<<"_";
	outName<<currentConfig_.min_dthreshold<<"_"<<currentConfig_.use_fed<<"_";
	outName<<currentConfig_.diffusivity<<"_"<<currentConfig_.sderivatives<<"_";
	outName<<currentConfig_.kcontrast<<"_"<<currentConfig_.kcontrast_percentile<<"_";
	outName<<currentConfig_.kcontrast_nbins;
	return outName.str();
}



	
	
	
}
