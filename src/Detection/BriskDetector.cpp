 
#include "vSLAM_FrontEnd/Detection/BriskDetector.hpp"

namespace stereo {

BriskDetector::BriskDetector(int thresh, int oct,float pattern)
{
	startConfig_.thresh_=thresh;
	startConfig_.octave_=oct;
	startConfig_.pattern_=pattern;
	updateSettings(startConfig_);
	currentConfig_=startConfig_;
}

void BriskDetector::detect(cv::Mat image, std::vector< cv::KeyPoint >& output)
{
	detector_->detect(image,output);
}

void BriskDetector::resetDefaults()
{
	currentConfig_=startConfig_;
}


void BriskDetector::adjustSettings()
{

}

void BriskDetector::updateSettings(settings_ &n)
{
	detector_.release();
	detector_= new cv::BRISK(n.thresh_,n.octave_,n.pattern_);
}


std::string BriskDetector::getName()
{
	std::stringstream outName;
	outName<<"BRISK_"<<currentConfig_.thresh_<<"_"<<currentConfig_.octave_<<"_"<<currentConfig_.pattern_;
	return outName.str();
}
	
	
}
