/*
 * Binary executable that takes an XML input directory to a single camera,
 *and estimates the calibration given the parameters specified in the XML*/

#include <opencv2/core/core.hpp>
#include "Structures/DataSet/BumbleDataSet.hpp"
#include "Structures/vSLAM/StereoFeatures.hpp"
#define DEFAULT_RECT "/home/ryan/git/groundTruth/gt/output/Stereo4/RectifiedBumble4.xml"
#include "vSLAM_FrontEnd/Detection/OrbDetector.hpp"
#include "dirUtilities.hpp"

#include <opencv2/highgui.hpp>

using namespace std;
int main(int argc, char **argv)
{
	if(argc!=4)
	{
		std::cerr << std::endl << "Incorrect Number of Parameters -example usage -->" << std::endl;
	    std::cerr <<"./saveOrbDataset [inputDataSetName] [outputDirectory] [DetectorName]" << std::endl;
	    std::cerr <<"./saveOrbDataset D1 ~/out ORB" << std::endl;
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
	
	int nmax=10000;//want to retain as many features as possible
	int firstVect=0;
	std::vector<float> scale_vect;
	std::vector<int> level_vect;
	std::vector<int> edge_vect;
	std::vector<int> wta_vect;
	std::vector<int> score_vect;
	std::vector<int> patch_vect;
	
	for(int index=1; index<5;index++)
	{
		scale_vect.push_back(0.1*index);
	}
	for(int index=1;index<5;index++)
	{
		level_vect.push_back(index);
	}
	for(int index=1;index<5;index++)
	{
		edge_vect.push_back(index);
	}
	for(int index=0;index<4;index++)
	{
		wta_vect.push_back(index);
	}
	
	score_vect.push_back((ORB::kBytes);
	score_vect.push_back(ORB::HARRIS_SCORE);
	score_vect.push_back(ORB::FAST_SCORE);
	
	for(int index=1;index<5;index++)
	{
		patch_vect.push_back(index);
	}
	

	DetectorSettings *CurrentDetector_;
	
/*	for(int scaleindex=0;scaleindex<scale_vect.size();scaleindex++)
	{
		for(int levelindex=0;levelindex<level_vect.size();levelindex++)
		{
			for(int edgeindex=0;edgeindex<edge_vect.size();edgeindex++)
			{
				for(int wtaindex=0;wtaindex<wta_vect.size();wtaindex++)
				{
					for(int scoreindex=0;scoreindex<score_vect.size();scoreindex++)
					{
						for(int pathindex=0;patchindex<patch_vect.size();patchindex++)
						{
									OrbDetector det(nmax,scale_vect.at(scaleindex),level_vect.at(levelindex),
																	edge_vect.at(edgeindex),firstVect,wta_vect.at(wtaindex),
																	score_vect.at(scoreindex),patch_vect.at(pathindex));

								CurrentDetector_=&det;
						}
					}
				}
			}
		}
	}*/
	
	
	/*
for(int scaleindex=0;scaleindex<scale_vect.size();scaleindex++)
	{
		for(int levelindex=0;levelindex<level_vect.size();levelindex++)
		{
			for(int edgeindex=0;edgeindex<edge_vect.size();edgeindex++)
			{
				for(int wtaindex=0;wtaindex<wta_vect.size();wtaindex++)
				{
					for(int scoreindex=0;scoreindex<score_vect.size();scoreindex++)
					{
						for(int pathindex=0;patchindex<patch_vect.size();patchindex++)
						{
								OrbDetector det(nmax,scale_vect.at(scaleindex),level_vect.at(levelindex),
																	edge_vect.at(edgeindex),firstVect,wta_vect.at(wtaindex),
																	score_vect.at(scoreindex),patch_vect.at(pathindex));

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
		}
	}
	*/
	
	return 0;
}
 
 
 
