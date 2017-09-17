#include "vSLAM_FrontEnd/StereoFront/Matching.hpp"

namespace stereo {
	
Matching::Matching()
{
	loweRatio=0.8;
	epiThreshold=2;
}


void Matching::knnMatch(StereoFrame& in, std::vector< std::vector< cv::DMatch > >& out)
{
	cv::BFMatcher m(cv::NORM_HAMMING,false);
	m.knnMatch(in.leftDescrip_,in.rightDescrip_,out,2);
}


void Matching::stereoBruteMatch(StereoFrame& in,std::vector<cv::DMatch> &out)
{
	//assuming ORB for now
	cv::BFMatcher m(cv::NORM_HAMMING,false);
	m.match(in.leftDescrip_,in.rightDescrip_,out);
	//m.knnMatch(in.leftDescrip_,in.rightDescrip_,out,5);
}


void Matching::filterLoweRatio(std::vector< std::vector< cv::DMatch > > &in,std::vector<char> &filter,std::vector<cv::DMatch> &out)
{
	std::vector<char>::iterator end,it;
	it=filter.begin();
	end=filter.end();
	for(;it!=end;++it)
	{
		if( in.at(std::distance(filter.begin(),it)).at(0).distance < loweRatio*in.at(std::distance(filter.begin(),it)).at(1).distance)
		{
			if(*it==1)
			{
				out.push_back(in.at(std::distance(filter.begin(),it)).at(0));
			}
		}
		else
		{
			*it=0;
		}

	}
	
}

void Matching::filterEpiPolar(std::vector< cv::DMatch >& in, std::vector< cv::DMatch >& out)
{
	
}




	
	
}


