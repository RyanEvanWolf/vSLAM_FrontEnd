/*
 * Binary executable that takes an XML input directory to a single camera,
 *and estimates the calibration given the parameters specified in the XML*/

#include <opencv2/core/core.hpp>
#include "Structures/DataSet/BumbleDataSet.hpp"
#include "Structures/vSLAM/StereoFeatures.hpp"
#define DEFAULT_RECT "/home/ryan/git/groundTruth/gt/output/Stereo4/RectifiedBumble4.xml"
#include "vSLAM_FrontEnd/Detection/BriskDetector.hpp"
#include "dirUtilities.hpp"

#include <opencv2/highgui.hpp>

using namespace std;
int main(int argc, char **argv)
{
	if(argc!=4)
	{
		std::cerr << std::endl << "Incorrect Number of Parameters -example usage -->" << std::endl;
	    std::cerr <<"./saveBriskDataset [inputDataSetName] [outputDirectory] [DetectorName]" << std::endl;
	    std::cerr <<"./saveBriskDataset D1 ~/out BRISK" << std::endl;
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
	std::vector<int> oct_vect;
	std::vector<float> pat_vect;
		
	for(int index=1;index<5;index++)
	{
		thresh_vect.push_back(index);
	}
	for(int index=1;index<5;index++)
	{
		oct_vect.push_back(index);
	}
	for(int index=1;index<5;index++)
	{
		pat_vect.push_back(index*0.1);
	}
	

	DetectorSettings *CurrentDetector_;
	
	for(int indexThresh=0;indexThresh<thresh_vect.size();indexThresh++)
	{
		for(int indexOct=0;indexOct<oct_vect.size();indexOct++)
		{
			for(int indexPat=0;indexPat<pat_vect.size();indexPat++)
			{
				std::cout<<"newBrisk sequence beginning"<<std::endl;
				BriskDetector det(thresh_vect.at(indexThresh),
								 oct_vect.at(indexOct),
								 pat_vect.at(indexPat));
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
 
 
 
 
