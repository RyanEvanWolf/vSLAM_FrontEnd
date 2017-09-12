 
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
	//currentThreshold_=threshold_;
}


void KazeDetector::adjustSettings()
{

}

std::string KazeDetector::getName()
{
	std::stringstream outName;
	outName<<"KAZE_";//<<suppression_<<"_"<<type_<<"_"<<currentThreshold_;
	return outName.str();
}



	
	
	
}
