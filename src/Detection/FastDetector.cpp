#include "vSLAM_FrontEnd/Detection/FastDetector.hpp"


namespace stereo {
	
FastDetector::FastDetector(int initialThreshold_,bool suppress,int patternType)
{
	threshold_=initialThreshold_;
	currentThreshold_=initialThreshold_;
	suppression_=suppress;
	type_=patternType;
}

void FastDetector::detect(cv::Mat image, std::vector< cv::KeyPoint >& output)
{
	cv::FASTX(image,output,threshold_,suppression_,type_);
}

void FastDetector::resetDefaults()
{
	currentThreshold_=threshold_;
}


void FastDetector::adjustSettings()
{

}

std::string FastDetector::getName()
{
	std::stringstream outName;
	outName<<"FAST_"<<suppression_<<"_"<<type_<<"_"<<currentThreshold_;
	return outName.str();
}



	
	
	
}
