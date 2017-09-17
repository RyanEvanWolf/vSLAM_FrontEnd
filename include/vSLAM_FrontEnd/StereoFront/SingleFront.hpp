#ifndef FRONT_END_SINGLE_FEATURE_HPP_
#define FRONT_END_SINGLE_FEATURE_HPP_

#include "vSLAM_FrontEnd/Detection/OrbDetector.hpp"

namespace stereo
{

class SingleFront
{
	public:
		//still to add grid + adaptive detection
		cv::Ptr<DetectorSettings> detection_;
		cv::Ptr<DetectorSettings> description_;	
		SingleFront(cv::Ptr<DetectorSettings> det,cv::Ptr<DetectorSettings> descr);
};

	
}

#endif
