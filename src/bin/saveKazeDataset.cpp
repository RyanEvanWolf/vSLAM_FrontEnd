/*
 * Binary executable that takes an XML input directory to a single camera,
 *and estimates the calibration given the parameters specified in the XML*/

#include <opencv2/core/core.hpp>
#include "Structures/DataSet/BumbleDataSet.hpp"
#include "Structures/vSLAM/StereoFeatures.hpp"
#define DEFAULT_RECT "/home/ryan/git/Output/Calibration/stereo_ParameterFour.xml"
#include "vSLAM_FrontEnd/Detection/KazeDetector.hpp"
#include "dirUtilities.hpp"

#include <opencv2/highgui.hpp>

using namespace std;
int main(int argc, char **argv)
{
	if(argc!=4)
	{
		std::cerr << std::endl << "Incorrect Number of Parameters -example usage -->" << std::endl;
	    std::cerr <<"./saveKazeDataset [inputDataSetName] [outputDirectory] [DetectorName]" << std::endl;
	    std::cerr <<"./saveKazeDataset D1 ~/out KAZE" << std::endl;
        return 1;
	}
	
/*	SaveDirectoryInfo outConfig=getInfo(argc,argv);
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
	
	std::vector<float> offset_vect;
	std::vector<int> omax_vect;
	std::vector<int> subl_vect;
	
	float minthresh=0.00001;
	std::vector<float> dthresh_vect;
	for(int index=1;index<6;index++)
	{
		dthresh_vect.push_back(index*minthresh);
	}

	std::vector<bool> fed;
	fed.push_back(true);
	fed.push_back(false);
	
	std::vector<int> diffuse_vect;
	std::vector<float> sder_vect;
	std::vector<float> kcontr_vect;
	std::vector<float> kperc_vect;
	std::vector<float> kbin_vect;
	
	*/
	


	//DetectorSettings *CurrentDetector_;
	
	cv::Mat testImage;
	cv::imread("/home/ryan/Figure_1.png",CV_LOAD_IMAGE_GRAYSCALE);
	
	KAZEOptions settings;
	std::cout<<settings.soffset<<endl;
	
	stereo::KazeDetector mine(settings);
	std::vector<cv::KeyPoint> a;
	
	mine.detect(testImage,a);
	std::cout<<a.size()<<endl;
	
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
						for(int patchindex=0;patchindex<patch_vect.size();patchindex++)
						{
								OrbDetector det(nmax,scale_vect.at(scaleindex),level_vect.at(levelindex),
																	edge_vect.at(edgeindex),firstVect,wta_vect.at(wtaindex),
																	score_vect.at(scoreindex),patch_vect.at(patchindex));

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
						}
					}
				}
			}
		}
	}
*/
	cv::destroyAllWindows();
	return 0;
}
 
 
 
 
