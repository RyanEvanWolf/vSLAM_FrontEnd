#include "vSLAM_FrontEnd/Detection/FreakDetector.hpp"

namespace stereo {
	
FreakDetector::FreakDetector(bool orient,bool scale, float pattern,int oct)
{
	startConfig_.orientNormal_=orient;
	startConfig_.scaleNormal_=scale;
	startConfig_.patternScale_=pattern;
	startConfig_.octaves_=oct;
	
	updateSettings(startConfig_);
	currentConfig_=startConfig_;
}


void FreakDetector::detect(cv::Mat image, std::vector< cv::KeyPoint >& output)
{
	detector_->detect(image,output);
}

void FreakDetector::resetDefaults()
{
	currentConfig_=startConfig_;
}


void FreakDetector::adjustSettings()
{

}

void FreakDetector::updateSettings(settings_ &n)
{
	detector_.release();
	detector_= new cv::SIFT(n.nBest_,n.octave_,n.contrastThresh_,n.edgeThresh_,n.gaussSigma_);
}


std::string FreakDetector::getName()
{
	std::stringstream outName;
	outName<<"FREAK_"<<currentConfig_.orientNormal_<<"_"<<currentConfig_.scaleNormal_<<"_";
	outName<<currentConfig_.patternScale_<<"_"<<currentConfig_.octaves_;
	return outName.str();
}
	
	
}
