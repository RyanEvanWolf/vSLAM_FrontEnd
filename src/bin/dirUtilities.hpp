
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
	}
	return out;
}

void makeFullPath(SaveDirectoryInfo in)
{
	std::string command;
	command="mkdir -pv ";
	command+=in.directory[DirectoryNames::baseOutDir_];
	command+="/";
	command+=in.directory[DirectoryNames::DataSetName_];
	command+="/";
	command+=in.directory[DirectoryNames::DetectorName_];
	command+="/";
	command+=in.directory[DirectoryNames::DetectorSettings_];
	system(command.c_str());
}

std::string getFullOutPath(SaveDirectoryInfo in)
{
	std::string fullpath;
	fullpath=in.directory[DirectoryNames::baseOutDir_];
	fullpath+="/";
	fullpath+=in.directory[DirectoryNames::DataSetName_];
	fullpath+="/";
	fullpath+=in.directory[DirectoryNames::DetectorName_];
	
	if(!in.directory[DirectoryNames::DetectorSettings_].empty())
	{
		fullpath+="/";
		fullpath+=in.directory[DirectoryNames::DetectorSettings_];
	}
	else
	{
		return fullpath;
	}
}
