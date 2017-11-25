/*
 * Binary executable that takes an XML input directory to a single camera,
 *and estimates the calibration given the parameters specified in the XML*/

#include <opencv2/core/core.hpp>
#include <opencv2/highgui.hpp>
#include <vSLAM_FrontEnd/Detection/KazeDetector.hpp>

using namespace std;
int main(int argc, char **argv)
{
	if(argc!=2)
	{
		std::cerr << std::endl << "Incorrect Number of Parameters -example usage -->" << std::endl;
	    std::cerr <<"./displayKAZE [imageDir]" << std::endl;
        return 1;
	}
	
	
	cv::namedWindow("KAZE",CV_WINDOW_FREERATIO);
	string fileName(argv[1]);
	
	cv::Mat inImage;
	inImage=cv::imread(fileName,CV_LOAD_IMAGE_GRAYSCALE);
	
	KAZEOptions options;
	
	// Convert the image to float to extract features
	cv::Mat img_32;
	inImage.convertTo(img_32, CV_32F, 1.0/255.0, 0);

	// Don't forget to specify image dimensions in AKAZE's options
	options.img_width = inImage.cols;
	options.img_height = inImage.rows;

	// Extract features
	stereo::KazeDetector evolution(options);
	vector<cv::KeyPoint> kpts;
	
	evolution.detect(inImage,kpts);

	//evolution.Create_Nonlinear_Scale_Space(img_32);
	//evolution.Feature_Detection(kpts);
	
	cv::Mat out;
	cv::drawKeypoints(inImage,kpts,out);
	
	cv::imshow("KAZE",out);
	cv::waitKey(0);
	cv::destroyAllWindows();
	return 0;
}
 
 
 
 
 
