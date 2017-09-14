/*
 * Binary executable that takes an XML input directory to a single camera,
 *and estimates the calibration given the parameters specified in the XML*/

#include <opencv2/core/core.hpp>
#include "Structures/DataSet/BumbleDataSet.hpp"
#include "Structures/vSLAM/StereoFeatures.hpp"
#define DEFAULT_RECT "/home/ryan/git/groundTruth/gt/output/Stereo4/RectifiedBumble4.xml"
#include "vSLAM_FrontEnd/Detection/FreakDetector.hpp"
#include "dirUtilities.hpp"

#include <opencv2/highgui.hpp>

using namespace std;
int main(int argc, char **argv)
{
	if(argc!=4)
	{
		std::cerr << std::endl << "Incorrect Number of Parameters -example usage -->" << std::endl;
	    std::cerr <<"./saveFreakDataset [inputDataSetName] [outputDirectory] [DetectorName]" << std::endl;
	    std::cerr <<"./saveFreakDataset D1 ~/out FREAK" << std::endl;
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

	std::vector<bool> bool_vect;
	std::vector<float> patt_vect;
	std::vector<int> oct_vect;
	
	bool_vect.push_back(true);
	bool_vect.push_back(false);
	
	for(int index=0;index<5;index++)
	{
		patt_vect.push_back(1.0*index);
	}
	
	for(int index=0;index<5;index++)
	{
		oct_vect.push_back(index);
	}
	DetectorSettings *CurrentDetector_;
	
	
	/*for(int orientindex=0;orientindex<2;orientindex++)
	{
		for(int scaleindex=0;scaledindex<2;scaleindex++)
		{
			for(int patindex=0;patindex<patt_vect.size();patindex++)
			{
				for(int octindex=0;octindex<oct_vect.size();octindex++)
				{
					FreakDetector det(bool_vect.at(orientindex),bool_vect.at(orientindex),
														patt_vect.at(patindex),oct_vect.at(octindex));
					CurrentDetector_=&det;
				}
			}
		}
	}*/
	
	/*
	for(int orientindex=0;orientindex<2;orientindex++)
	{
		for(int scaleindex=0;scaledindex<2;scaleindex++)
		{
			for(int patindex=0;patindex<patt_vect.size();patindex++)
			{
				for(int octindex=0;octindex<oct_vect.size();octindex++)
				{
					FreakDetector det(bool_vect.at(orientindex),bool_vect.at(orientindex),
														patt_vect.at(patindex),oct_vect.at(octindex));
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
						if(im>5)
						{
							end=true;
						}
						std::cout<<im<<std::endl;
					}
					cv::destroyAllWindows();
				
				}
			}
		}
	}
	
	*/

	return 0;
}
 
 
 
