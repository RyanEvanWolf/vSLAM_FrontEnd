/*
 * Binary executable that takes an XML input directory to a single camera,
 *and estimates the calibration given the parameters specified in the XML*/

#include <opencv2/core/core.hpp>
#include "Structures/DataSet/BumbleDataSet.hpp"
#include "Structures/vSLAM/StereoFeatures.hpp"
#define DEFAULT_DATASET "/home/ryan/DataSets/Mine/Decent-cloudy/recording-20170702-122129-924"
#define DEFAULT_RECT "/home/ryan/git/groundTruth/gt/output/Stereo4/RectifiedBumble4.xml"
#include "vSLAM_FrontEnd/Detection/FastDetector.hpp"


#include <opencv2/highgui.hpp>

using namespace std;

//void LoadImages(const string &strPathLeft, const string &strPathRight, const string &strPathTimes,
//                vector<string> &vstrImageLeft, vector<string> &vstrImageRight, vector<double> &vTimeStamps);

int main(int argc, char **argv)
{
	std::string inputDataSetDir;
	std::string inputRectificationFile;
    
    if(argc==1)
	{
		//use default values
		inputDataSetDir=DEFAULT_DATASET;
		inputRectificationFile=DEFAULT_RECT;
	}
	else if(argc==3)
	{
		//use the command line arguments
		inputDataSetDir=std::string(argv[1]);
		inputRectificationFile=std::string(argv[2]);
	}
	else
	{
		std::cerr << endl << "Incorrect Number of Parameters -example usage -->" << endl;
	    std::cerr <<"./ORB_exec [rectification.xml] [dataSetDir]" << endl;
	    std::cerr <<"./ORB_exec" << endl;
        return 1;
	}
	

	
	using namespace stereo;
	
	//initialize the BumbleBee dataset and camera
	BumbleDataSet bumbleData(inputDataSetDir,inputRectificationFile);
	
	cv::FileStorage c(inputRectificationFile,cv::FileStorage::READ);
	
	if(!c.isOpened())
	{
		std::cout<<"Stereo Configuration file not found\n";
		return -1;
	}
	StereoRect bumbleBee_;
	c["StereoRect"]>>bumbleBee_;
	c.release();
	
	//initialize the feature Exraction
	FastDetector det_(35,false,cv::FastFeatureDetector::TYPE_9_16);

	
	bool end=false;
	
	cv::Mat leftUndistorted=cv::Mat(bumbleBee_.L_fMapx_.size(),bumbleData.getCurrentLeft().type());
	cv::Mat rightUndistorted=cv::Mat(bumbleBee_.L_fMapx_.size(),bumbleData.getCurrentLeft().type());
	
	cv::Mat leftIN=leftUndistorted(bumbleBee_.l_ROI_);
	cv::Mat rightIN=rightUndistorted(bumbleBee_.r_ROI_);
	cv::Mat leftimg_,rightimg_;
	
	int im=0;
	cv::namedWindow("l",cv::WINDOW_NORMAL);
	cv::namedWindow("r",cv::WINDOW_NORMAL);
	while(!end)
	{
		cv::Mat outl,outr;
		StereoFeatures Current_;
		double tframe=(1000/15)*im;
		//undistort each image, and set them to the region of interest
		//cv::Mat inputLeft=bumbleData.getCurrentLeft()
		cv::remap(bumbleData.getCurrentLeft(),leftUndistorted,bumbleBee_.L_fMapx_,bumbleBee_.L_fMapy_,cv::INTER_LINEAR);
		cv::remap(bumbleData.getCurrentRight(),rightUndistorted,bumbleBee_.R_fMapx_,bumbleBee_.R_fMapy_,cv::INTER_LINEAR);

		det_.detect(leftIN,Current_.leftFeatures);
		det_.detect(rightIN,Current_.rightFeatures);
		
		cv::drawKeypoints(leftIN,Current_.leftFeatures,outl);
		cv::drawKeypoints(rightIN,Current_.rightFeatures,outr);
		
		cv::imshow("l",outl);
		cv::imshow("r",outr);
		
		cv::FileStorage a("out.xml",cv::FileStorage::WRITE);
		a<<"out"<<Current_;
		a.release();
		
		
		cv::waitKey(100/15);
		end= !bumbleData.incrementFrame();
		im++;
		if(im>100)
		{
			end=true;
		}
		std::cout<<im<<std::endl;
	}
	cv::destroyAllWindows();
	return 0;
}
 
 
