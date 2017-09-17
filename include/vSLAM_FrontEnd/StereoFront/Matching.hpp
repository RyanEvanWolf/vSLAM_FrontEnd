#ifndef STEREO_FRONT_MATCHING_HEADER_HPP
#define STEREO_FRONT_MATCHING_HEADER_HPP

#include "Structures/CameraInfo/StereoRect.hpp"
#include "vSLAM_FrontEnd/Detection/DetectorSettings.hpp"
#include "Structures/vSLAM/StereoFrame.hpp"

namespace stereo 
{
	class Matching 
	{
		public:
			float loweRatio;
			float epiThreshold;
			Matching();
			void stereoBruteMatch(StereoFrame &in,std::vector<cv::DMatch> &out);
			void knnMatch(StereoFrame &in,std::vector<std::vector<cv::DMatch>> &out);
			void filterLoweRatio(std::vector< std::vector< cv::DMatch > > &in,std::vector<char> &filter,std::vector<cv::DMatch> &out);
			void filterEpiPolar(std::vector< cv::DMatch >&in,std::vector<cv::DMatch> &out);
	};
	
	
}



#endif
