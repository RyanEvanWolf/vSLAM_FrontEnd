#include "vSLAM_FrontEnd/Detection/OrbDetector.hpp"

namespace stereo {
	
OrbDetector::OrbDetector(int nMax,float scaleFactor,int nlevels,int edgeThresh,int firstLevel,int WTA_K,
								 int score_type,int patchSize)
{
	baseName_=DetectorSettings::ORB;
	startConfig_.nMax_=nMax;
	startConfig_.scaleFactor_=scaleFactor;
	startConfig_.nlevels_=nlevels;
	startConfig_.edgeThresh_=edgeThresh;
	startConfig_.firstLevel_=firstLevel;
	startConfig_.WTA_K_=WTA_K;
	startConfig_.score_type_=score_type;
	startConfig_.patchSize_=patchSize;
	
	updateSettings(startConfig_);
	currentConfig_=startConfig_;
}


void OrbDetector::detect(cv::Mat image, std::vector< cv::KeyPoint >& output)
{
	detector_->detect(image,output);
}

void OrbDetector::resetDefaults()
{
	currentConfig_=startConfig_;
}


void OrbDetector::adjustSettings()
{

}

void OrbDetector::updateSettings(settings_ &n)
{
	detector_.release();
	detector_= new cv::ORB(n.nMax_,n.scaleFactor_,n.nlevels_,
						   n.edgeThresh_,n.firstLevel_,n.WTA_K_,
						   n.score_type_,n.patchSize_);
}


std::string OrbDetector::getName()
{
	std::stringstream outName;
	outName<<"ORB_"<<currentConfig_.nMax_<<"_"<<currentConfig_.scaleFactor_<<"_"<<currentConfig_.nlevels_;
	outName<<"_"<<currentConfig_.edgeThresh_<<"_"<<currentConfig_.firstLevel_;
	outName<<"_"<<currentConfig_.WTA_K_<<"_"<<currentConfig_.score_type_<<"_"<<currentConfig_.patchSize_;
	return outName.str();
}

OrbDetector::settings_ OrbDetector::getFromFileName(std::__cxx11::string in)
{
	settings_ output;
	return output;
}


	
void OrbDetector::extract(cv::Mat image, std::vector< cv::KeyPoint >& in, cv::Mat& out)
{
	detector_->operator()(image,cv::Mat(),in,out,true);
}


	
}
