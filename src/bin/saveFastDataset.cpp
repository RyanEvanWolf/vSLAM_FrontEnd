/*
 * Binary executable that takes an XML input directory to a single camera,
 *and estimates the calibration given the parameters specified in the XML*/

#include <opencv2/core/core.hpp>
#include "Structures/DataSet/BumbleDataSet.hpp"
#include "Structures/vSLAM/StereoFeatures.hpp"
#define DEFAULT_RECT "/home/ryan/git/groundTruth/gt/output/Stereo4/RectifiedBumble4.xml"
#include "vSLAM_FrontEnd/Detection/FastDetector.hpp"
#include "dirUtilities.hpp"

#include <opencv2/highgui.hpp>

using namespace std;
int main(int argc, char **argv)
{
	if(argc!=4)
	{
		std::cerr << std::endl << "Incorrect Number of Parameters -example usage -->" << std::endl;
	    std::cerr <<"./saveFastDataset [inputDataSetName] [outputDirectory] [DetectorName]" << std::endl;
	    std::cerr <<"./saveFastDataset D1 ~/out FAST" << std::endl;
        return 1;
	}
	
	SaveDirectoryInfo outConfig=getInfo(argc,argv);
	//create all the possible variations of the SiftDetector
	
	using namespace stereo;
	
	//initialize the BumbleBee dataset and camera
	BumbleDataSet bumbleData(outConfig.directory[DirectoryNames::inputDirectory]);
	
	cv::FileStorage c(DEFAULT_RECT,cv::FileStorage::READ);
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
	
	for(int index=5;index<8;index++)
	{
		thresh_vect.push_back(index);
	}
	

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
				std::cout<<"newFast sequence beginning"<<std::endl;
				FastDetector det(thresh_vect.at(indexThresh),
								 suppress_vect.at(indexBool),
								 fastType.at(indexType));
				CurrentDetector_=&det;
				
				outConfig.directory[DirectoryNames::DetectorSettings_]=det.getName();
				makeFullPath(outConfig);
				
				//loop through the Dataset
				bool end=false;
				int im=0;

				while(!end)
				{
					cv::Mat outl,outr;
					StereoFeatures Current_;
					Current_.detectionSettings_=det.getName();
					double tframe=(1000/15)*im;
					//undistort each image, and set them to the region of interest
					//cv::Mat inputLeft=bumbleData.getCurrentLeft()
					cv::remap(bumbleData.getCurrentLeft(),leftUndistorted,bumbleBee_.L_fMapx_,bumbleBee_.L_fMapy_,cv::INTER_LINEAR);
					cv::remap(bumbleData.getCurrentRight(),rightUndistorted,bumbleBee_.R_fMapx_,bumbleBee_.R_fMapy_,cv::INTER_LINEAR);
					
					CurrentDetector_->detect(leftIN,Current_.leftFeatures);
					CurrentDetector_->detect(rightIN,Current_.rightFeatures);
		
					cv::drawKeypoints(leftIN,Current_.leftFeatures,outl);
					cv::drawKeypoints(rightIN,Current_.rightFeatures,outr);
		
					cv::imshow("l",outl);
					cv::imshow("r",outr);
					
					std::string outputName;
					outputName=getFullOutPath(outConfig);
					outputName+="/";
					outputName+=bumbleData.getCurrentName().erase(bumbleData.getCurrentName().length()-4);
					outputName+=".xml";
					
					Current_.frameData_=bumbleData.getCurrentMeta();
						
					std::cout<<outputName<<std::endl;
					cv::FileStorage a(outputName.c_str(),cv::FileStorage::WRITE);
					a<<"features"<<Current_;
					a.release();
		
					
					cv::waitKey(1);
					end= !bumbleData.incrementFrame();
					im++;
					std::cout<<"["<<im<<"/"<<bumbleData.getTotalImages()<<"]"<<std::endl;
				}
				std::cout<<"ResetSequence"<<std::endl;
				bumbleData.resetCurrent();
				
			}
		}
	}
	std::cout<<"Finished Dataset"<<std::endl;
	cv::destroyAllWindows();
	return 0;
}
 
 
 
