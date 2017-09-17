#include "vSLAM_FrontEnd/Detection/SiftDetector.hpp"

namespace stereo {

SiftDetector::SiftDetector(int nfeatures, int nOctaveLayers, double contrastThreshold, double edgeThreshold, double sigma)
{
	
	startConfig_.nBest_=nfeatures;
	startConfig_.octave_=nOctaveLayers;
	startConfig_.contrastThresh_=contrastThreshold;
	startConfig_.edgeThresh_=edgeThreshold;
	startConfig_.gaussSigma_=sigma;
	updateSettings(startConfig_);
	currentConfig_=startConfig_;
}

SiftDetector::SiftDetector()
{
	startConfig_.nBest_=0;
	startConfig_.octave_=3;
	startConfig_.contrastThresh_=0.04;
	startConfig_.edgeThresh_=10;
	startConfig_.gaussSigma_=1.6;
	updateSettings(startConfig_);
	currentConfig_=startConfig_;
}


void SiftDetector::detect(cv::Mat image, std::vector< cv::KeyPoint >& output)
{
	detector_->detect(image,output);
}

void SiftDetector::resetDefaults()
{
	currentConfig_=startConfig_;
}


void SiftDetector::adjustSettings()
{

}

void SiftDetector::updateSettings(settings_ &n)
{
	detector_.release();
	detector_= new cv::SIFT(n.nBest_,n.octave_,n.contrastThresh_,n.edgeThresh_,n.gaussSigma_);
}


std::string SiftDetector::getName()
{
	std::stringstream outName;
	outName<<"SIFT_"<<currentConfig_.nBest_<<"_"<<currentConfig_.octave_<<"_"<<currentConfig_.contrastThresh_;
	outName<<"_"<<currentConfig_.edgeThresh_<<"_"<<currentConfig_.gaussSigma_;
	return outName.str();
}

void SiftDetector::extract(cv::Mat image, std::vector< cv::KeyPoint >& in, cv::Mat& out)
{
	detector_->operator()(image,cv::Mat(),in,out,true);
}

	
	
}
