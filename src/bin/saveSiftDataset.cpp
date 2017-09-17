/*
 * Binary executable that takes an XML input directory to a single camera,
 *and estimates the calibration given the parameters specified in the XML*/

#include <opencv2/core/core.hpp>
#include <opencv2/highgui.hpp>

#include "Structures/DataSet/BumbleDataSet.hpp"
#include "Structures/vSLAM/StereoFeatures.hpp"
#define DEFAULT_RECT "/home/ryan/git/groundTruth/gt/output/Stereo4/RectifiedBumble4.xml"
#include "vSLAM_FrontEnd/Detection/SiftDetector.hpp"
#include "dirUtilities.hpp"


int main(int argc, char **argv)
{
	if(argc!=4)
	{
		std::cerr << std::endl << "Incorrect Number of Parameters -example usage -->" << std::endl;
	    std::cerr <<"./saveSiftDataset [inputDataSetName] [outputDirectory] [DetectorName]" << std::endl;
	    std::cerr <<"./saveSiftDataset D1 ~/out SIFT" << std::endl;
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
	
	//Create the bruteForce  Sift Parameters 
	
	
	std::stringstream tempString;
	std::vector<int> oct_;
	std::vector<double> contrast_;
	std::vector<double> edge_;
	std::vector<double> sigma_;
	
	for(int index=1;index<2;index++)
	{
		oct_.push_back(index);
	}
	for(int index=0;index<2;index++)
	{
		edge_.push_back(index*0.1);
	}
	for(int index=0;index<2;index++)
	{
		contrast_.push_back(index*0.5);
	}
	
	for(int index=1;index<2;index++)
	{
		sigma_.push_back(index*0.1);
	}
	
	DetectorSettings *CurrentDetector_;
	//cycle through the various detectors
	
	for(int octindex=0;octindex<oct_.size();octindex++)
	{
		for(int edgeindex=0;edgeindex<edge_.size();edgeindex++)
		{
			for(int contindex=0;contindex<contrast_.size();contindex++)
			{
				for(int sigindex=0;sigindex<sigma_.size();sigindex++)
				{
					std::cout<<"newSift sequence beginning"<<std::endl;
					SiftDetector det(0,oct_.at(octindex),contrast_.at(contindex),
									 edge_.at(edgeindex),sigma_.at(sigindex));
					CurrentDetector_=&det;
					outConfig.directory[DirectoryNames::DetectorSettings_]=det.getName();
					makeFullPath(outConfig);

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
		
	}
	std::cout<<"Finished Dataset"<<std::endl;
	cv::destroyAllWindows();
	return 0;
}
 
 
 
