#include "vSLAM_FrontEnd/StereoFront/Matching.hpp"

namespace stereo {
	
Matching::Matching()
{
	loweRatio=0.8;
	epiThreshold=2;
	pixelBox=40;
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
	std::vector< std::vector<cv::DMatch> >::iterator end,it,start;
	start=in.begin();
	it=in.begin();
	end=in.end();
	for(;it!=end;++it)
	{
		out.push_back((*it).at(0));
		if( (*it).at(0).distance < loweRatio*((*it).at(1).distance))
		{
			filter.at(std::distance(start,it));
		}
		else
		{
			filter.at(std::distance(start,it))=0;
		}
	}
	
}

void Matching::filterEpiPolar(StereoFrame& inOut)
{
	std::vector<char>::iterator start,end,it;
	start=inOut.inliersMask_.begin();
	it=inOut.inliersMask_.begin();
	end=inOut.inliersMask_.end();
	for(;it!=end;++it)
	{
		//get left points 
		if((*it)==1)
		{
			//getEpiDistance
			float error;
			error=inOut.leftFeatures_.at(inOut.matches_.at(std::distance(start,it)).queryIdx).pt.y;//inOut.matches_.at(std::distance(start,it)).queryIdx);
			error=abs(error-inOut.rightFeatures_.at(inOut.matches_.at(std::distance(start,it)).trainIdx).pt.y);
			if(error>epiThreshold)
			{
				(*it)=0;
			}
		}
	}
	
}

std::vector< char > Matching::invertMask(std::vector< char > in)
{
	std::vector<char> out;
	for(int index=0;index<in.size();index++)
	{
		if(in.at(index)==1)
		{
			out.push_back(0);
		}
		else
		{
			out.push_back(1);
		}
	}
	return out;
}


float Matching::getRatio(std::vector< char > in)
{
	float inliers=0;
	for(int index=0;index<in.size();index++)
	{
		if(in.at(index)==1)
		{
			inliers+=1;
		}
	}
	return 100*inliers/(float(in.size()));
}

void Matching::getWindowMask(StereoFrame& in, cv::Mat& outMask)
{
	outMask.release();
	outMask=cv::Mat(in.leftFeatures_.size(),in.rightFeatures_.size(),CV_8U);
	for(int row;row<in.leftFeatures_.size();row++)
	{
		for(int col;col<in.rightFeatures_.size();col++)
		{
			if(abs(in.leftFeatures_.at(row).pt.x-in.rightFeatures_.at(col).pt.x)<pixelBox)
			{
				if(abs(in.leftFeatures_.at(row).pt.y-in.rightFeatures_.at(col).pt.y)<pixelBox)
				{
					outMask.at<uchar>(row,col)=1;
				}
				else
				{
					outMask.at<uchar>(row,col)=0;
				}
			}
			else
			{
				outMask.at<uchar>(row,col)=0;
			}
		}
	}
}

void Matching::WindowMatch(StereoFrame& in)
{
	cv::Mat msk;
	getWindowMask(in,msk);
	
	for(int index=0;in.leftFeatures_.size();index++)
	{
		//cv::Mat descriptorRight
		
	}
	
	
	
	//assuming ORB for now
	cv::BFMatcher m(cv::NORM_HAMMING,true);
	m.match(in.leftDescrip_,in.rightDescrip_,in.matches_);
	
	in.inliersMask_.clear();
	for(int index=0;index<in.matches_.size();index++)
	{
		in.inliersMask_.push_back(1);
	}
}





	
	
}


