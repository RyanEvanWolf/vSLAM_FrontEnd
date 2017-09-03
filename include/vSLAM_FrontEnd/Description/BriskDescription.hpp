#ifndef FRONTEND_BRISK_DESCRIPTION_HEADER_HPP
#define FRONTEND_BRISK_DESCRIPTION_HEADER_HPP

#include "vSLAM_FrontEnd/Description/BaseDescription.hpp"

namespace stereo
{
	
class BriskDescription : public BaseDescription
{
	public:
		BriskDescription(int abc);
		void extract(cv::Mat image,cv::Mat &output) override;
	
};
	
}


#endif
