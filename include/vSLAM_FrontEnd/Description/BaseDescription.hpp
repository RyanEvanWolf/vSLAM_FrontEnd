#ifndef FRONTEND_BASE_DESCRIPTION_HEADER_HPP
#define FRONTEND_BASE_DESCRIPTION_HEADER_HPP

#include <iostream>
#include <opencv2/features2d.hpp>

namespace stereo
{
	
class BaseDescription
{
	public:
		virtual void extract(cv::Mat image,cv::Mat &output)=0;
		virtual ~BaseDescription(){};
	
};
	
}


#endif
 
