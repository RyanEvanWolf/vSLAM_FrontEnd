
#include <string>
#include <iostream>
#include <stdlib.h>
#define LAPTOP_DIRECTORY "/home/ryan/DataSets/SortedDataSets"

enum DirectoryNames
{
	inputDirectory=0,
	DataSetName_=1,
	baseOutDir_=2,
	DetectorName_=3,
	DetectorSettings_=4
};

struct SaveDirectoryInfo
{
	std::string directory[5];
};

SaveDirectoryInfo getInfo(int argc, char **argv)
{
	SaveDirectoryInfo out;
	if(argc==4)
	{
		out.directory[0]= std::string(LAPTOP_DIRECTORY)+"/";
		out.directory[0]+=argv[1];
		for(int index=1;index<4;index++)
		{
			out.directory[index]=std::string(argv[index]);
		}
		
		for(int index=0;index<5;index++)
		{
			std::cout<<out.directory[index]<<std::endl;
		}
	}
	return out;
}

void makeFullPath(SaveDirectoryInfo in)
{
	std::string command;
	command="mkdir -p ";
	command+=in.directory[DirectoryNames::baseOutDir_];
	command+="/";
	command+=in.directory[DirectoryNames::DataSetName_];
	command+="/";
	command+=in.directory[DirectoryNames::DetectorName_];
	command+="/";
	command+=in.directory[DirectoryNames::DetectorSettings_];
	std::cout<<command;
}
