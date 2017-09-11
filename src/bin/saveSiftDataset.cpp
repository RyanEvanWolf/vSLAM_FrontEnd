/*
 * Binary executable that takes an XML input directory to a single camera,
 *and estimates the calibration given the parameters specified in the XML*/

#include <opencv2/core/core.hpp>
#include "Structures/DataSet/BumbleDataSet.hpp"
#include "Structures/vSLAM/StereoFeatures.hpp"
#define DEFAULT_DATASET "/home/ryan/DataSets/Mine/Decent-cloudy/recording-20170702-122129-924"
#define DEFAULT_RECT "/home/ryan/git/groundTruth/gt/output/Stereo4/RectifiedBumble4.xml"
#define DEFAULT_FAST_OUT "/home/ryan/git/vSLAM_FrontEnd/output/CLOUDY/SIFT"
#define features.xml
#include "vSLAM_FrontEnd/Detection/SiftDetector.hpp"
#include "vSLAM_FrontEnd/Detection/ASiftDetector.h"

#include <opencv2/highgui.hpp>

using namespace std;

//void LoadImages(const string &strPathLeft, const string &strPathRight, const string &strPathTimes,
//                vector<string> &vstrImageLeft, vector<string> &vstrImageRight, vector<double> &vTimeStamps);

int main(int argc, char **argv)
{
	std::string inputDataSetDir;
	std::string inputRectificationFile;
	std::string outputDirectory;
    
    if(argc==1)
	{
		//use default values
		inputDataSetDir=DEFAULT_DATASET;
		inputRectificationFile=DEFAULT_RECT;
		outputDirectory=DEFAULT_FAST_OUT;
	}
	else if(argc==4)
	{
		//use the command line arguments
		inputDataSetDir=std::string(argv[1]);
		inputRectificationFile=std::string(argv[2]);
		outputDirectory=std::string(argv[3]);
	}
	else
	{
		std::cerr << endl << "Incorrect Number of Parameters -example usage -->" << endl;
	    std::cerr <<"./saveFastDataset [rectification.xml] [dataSetDir] [outDirectory]" << endl;
	    std::cerr <<"./saveFastDataset" << endl;
        return 1;
	}
	
	//create all the possible variations of the fastDetector
	
	using namespace stereo;
	
	//initialize the BumbleBee dataset and camera
	BumbleDataSet bumbleData(inputDataSetDir);
	
	cv::FileStorage c(inputRectificationFile,cv::FileStorage::READ);
	
	if(!c.isOpened())
	{
		std::cout<<"Stereo Configuration file not found\n";
		return -1;
	}
	StereoRect bumbleBee_;
	c["StereoRect"]>>bumbleBee_;
	c.release();
	
	cv::Mat leftUndistorted=cv::Mat(bumbleBee_.L_fMapx_.size(),bumbleData.getCurrentLeft().type());
	cv::Mat rightUndistorted=cv::Mat(bumbleBee_.R_fMapx_.size(),bumbleData.getCurrentLeft().type());
	
	cv::Mat leftIN=leftUndistorted(bumbleBee_.l_ROI_);
	cv::Mat rightIN=rightUndistorted(bumbleBee_.r_ROI_);
	cv::Mat leftimg_,rightimg_;
	
	//Create the various Parameters 
	
	
	std::stringstream tempString;
	std::vector<int> thresh_vect;
	std::vector<int> fastType;
	std::vector<bool> suppress_vect;
	
	
	fastType.push_back(cv::FastFeatureDetector::TYPE_5_8);
	fastType.push_back(cv::FastFeatureDetector::TYPE_7_12);
	fastType.push_back(cv::FastFeatureDetector::TYPE_9_16);
	
	suppress_vect.push_back(false);
	suppress_vect.push_back(true);
	
	for(int index=1;index<2;index++)
	{
		thresh_vect.push_back(index);
	}
	
	//Create main output directories
	tempString.str("");
	tempString<<"mkdir -pv "<<outputDirectory;
	system(tempString.str().c_str());
	
	DetectorSettings *CurrentDetector_;
	
	for(int indexThresh=0;indexThresh<thresh_vect.size();indexThresh++)
	{
		for(int indexType=0;indexType<fastType.size();indexType++)
		{
			for(int indexBool=0;indexBool<suppress_vect.size();indexBool++)
			{
				std::cout<<thresh_vect.at(indexThresh)<<std::endl;
				std::cout<<suppress_vect.at(indexBool)<<std::endl;
				std::cout<<fastType.at(indexType)<<std::endl;
				SiftDetector det;
				CurrentDetector_=&det;
				
				std::string dirName;
				std::string FullDir;
				
				dirName=CurrentDetector_->getName();
				
				FullDir=outputDirectory;
				FullDir+="/";
				FullDir+=dirName;
				
				tempString.str("");
				tempString<<"mkdir -pv "<<FullDir;
				system(tempString.str().c_str());
				//loop through the Dataset
				bool end=false;
				int im=0;

				while(!end)
				{
					cv::Mat outl,outr;
					StereoFeatures Current_;
					Current_.detectionSettings_=dirName;
					double tframe=(1000/15)*im;
					//undistort each image, and set them to the region of interest
					//cv::Mat inputLeft=bumbleData.getCurrentLeft()
					cv::remap(bumbleData.getCurrentLeft(),leftUndistorted,bumbleBee_.L_fMapx_,bumbleBee_.L_fMapy_,cv::INTER_LINEAR);
					cv::remap(bumbleData.getCurrentRight(),rightUndistorted,bumbleBee_.R_fMapx_,bumbleBee_.R_fMapy_,cv::INTER_LINEAR);
					
					CurrentDetector_->detect(leftIN,Current_.leftFeatures);
					CurrentDetector_->detect(rightIN,Current_.rightFeatures);
					
					cv::ASiftDetector aaa;
					cv::Mat r;
					std::vector<cv::KeyPoint> t;
					
					aaa.detectAndCompute(leftIN,t,r);
		
					cv::drawKeypoints(leftIN,Current_.leftFeatures,outl);
					cv::drawKeypoints(rightIN,Current_.rightFeatures,outr);
		
					cv::imshow("l",outl);
					cv::imshow("r",outr);
					
					std::string outputName;
					outputName=FullDir;
					outputName+="/";
					outputName+=bumbleData.getCurrentName().erase(bumbleData.getCurrentName().length()-4);
					outputName+=".xml";
					
					Current_.frameData_=bumbleData.getCurrentMeta();
					
					std::cout<<outputName<<std::endl;
					cv::FileStorage a(outputName,cv::FileStorage::WRITE);
					a<<"features"<<Current_;
					a.release();
		
					
					cv::waitKey(100/15);
					end= !bumbleData.incrementFrame();
					im++;
					std::cout<<im<<std::endl;
				}
				cv::destroyAllWindows();
				
			}
		}
	}
	return 0;
}
 
 
 
