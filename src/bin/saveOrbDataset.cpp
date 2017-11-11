/*
 * Binary executable that takes an XML input directory to a single camera,
 *and estimates the calibration given the parameters specified in the XML*/

#include <opencv2/core/core.hpp>
#include "Structures/DataSet/BumbleDataSet.hpp"
#define DEFAULT_RECT "/home/ryan/git/groundTruth/gt/output/Stereo4/RectifiedBumble4.xml"
#include "vSLAM_FrontEnd/Detection/OrbDetector.hpp"
#include "vSLAM_FrontEnd/StereoFront/StereoCamera.hpp"
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
	
	
	//Create the various Parameters 
	
	int nmax=10000;//want to retain as many features as possible
	int firstVect=0;
	std::vector<float> scale_vect;
	std::vector<int> level_vect;
	std::vector<int> edge_vect;
	std::vector<int> wta_vect;
	std::vector<int> score_vect;
	std::vector<int> patch_vect;
	
	for(int index=1; index<2;index++)
	{
		scale_vect.push_back(0.05*index);
	}
	for(int index=1;index<2;index++)
	{
		level_vect.push_back(index);
	}
	for(int index=1;index<2;index++)
	{
		edge_vect.push_back(index);
	}
	for(int index=2;index<5;index++)
	{
		wta_vect.push_back(index);
	}
	
	score_vect.push_back(cv::ORB::kBytes);
	score_vect.push_back(cv::ORB::HARRIS_SCORE);
	score_vect.push_back(cv::ORB::FAST_SCORE);
	
	for(int index=2;index<3;index++)
	{
		patch_vect.push_back(index);
	}
	

	DetectorSettings *CurrentDetector_;
	

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
							cv::Ptr<DetectorSettings> leftDetection= new OrbDetector(nmax,scale_vect.at(scaleindex),level_vect.at(levelindex),
																	edge_vect.at(edgeindex),firstVect,wta_vect.at(wtaindex),
																	score_vect.at(scoreindex),patch_vect.at(patchindex));
							cv::Ptr<DetectorSettings>  rightDetection= new OrbDetector(nmax,scale_vect.at(scaleindex),level_vect.at(levelindex),
																	edge_vect.at(edgeindex),firstVect,wta_vect.at(wtaindex),
																	score_vect.at(scoreindex),patch_vect.at(patchindex));
							cv::Ptr<DetectorSettings>  leftDescriptor= new OrbDetector(nmax,scale_vect.at(scaleindex),level_vect.at(levelindex),
																	edge_vect.at(edgeindex),firstVect,wta_vect.at(wtaindex),
																	score_vect.at(scoreindex),patch_vect.at(patchindex));
							cv::Ptr<DetectorSettings>  rightDescriptor= new OrbDetector(nmax,scale_vect.at(scaleindex),level_vect.at(levelindex),
																	edge_vect.at(edgeindex),firstVect,wta_vect.at(wtaindex),
																	score_vect.at(scoreindex),patch_vect.at(patchindex));
							StereoCamera Cam(leftDetection,rightDetection,
											 leftDescriptor,rightDescriptor,
											 DEFAULT_RECT);
							std::cout<<"newOrb sequence beginning"<<std::endl;
								
							outConfig.directory[DirectoryNames::DetectorSettings_]=leftDetection->getName();
							makeFullPath(outConfig);
				
							//loop through the Dataset
							bool end=false;
							int im=0;

							while(!end)
							{
								cv::Mat tttt,outtt;
								cv::Mat lP,rP;
								StereoFrame out;
								double tframe=(1000/15)*im;
								Cam.extractStereoFrame(bumbleData.getCurrentLeft(),bumbleData.getCurrentRight(),out);
								Cam.lroi_.copyTo(lP);
								Cam.rroi_.copyTo(rP);
								
								cv::drawKeypoints(Cam.lroi_,out.leftFeatures_,lP);
								cv::drawKeypoints(Cam.rroi_,out.rightFeatures_,rP);
								
								//add offset
								
								for(int index=0;index<out.leftFeatures_.size();index++)
								{
									out.leftFeatures_.at(index).pt.x+=Cam.cameraSettings_.l_ROI_.x;
									out.leftFeatures_.at(index).pt.y+=Cam.cameraSettings_.l_ROI_.y;
								}
								bumbleData.getCurrentLeft().copyTo(tttt);
								cv::remap(bumbleData.getCurrentLeft(),tttt,Cam.cameraSettings_.L_fMapx_,Cam.cameraSettings_.L_fMapy_,cv::INTER_LINEAR);
								cv::drawKeypoints(bumbleData.getCurrentLeft(),out.leftFeatures_,outtt);
								
								cv::imshow("l",lP);
								cv::imshow("r",rP);
								cv::imshow("out",outtt);
								cv::waitKey(0);
								std::string outputName;
								outputName=getFullOutPath(outConfig);
								outputName+="/";
								outputName+=bumbleData.getCurrentName().erase(bumbleData.getCurrentName().length()-4);
								outputName+=".xml";
					
								out.frameData_=bumbleData.getCurrentMeta();
						
								std::cout<<outputName<<std::endl;
								//cv::FileStorage a(outputName.c_str(),cv::FileStorage::WRITE);
								//a<<"features"<<out;
								//a.release();
									
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
		}
	}
	std::cout<<"Finished Dataset"<<std::endl;
	cv::destroyAllWindows();
	return 0;
}
 
 
 
