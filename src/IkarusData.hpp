#pragma once
#include "RoseData.hpp"
#include "RoseSkeletonLoader.hpp"
#include "RoseMeshList.hpp"

class IkarusData
{
public:
	int worldnum;
	int channum;
	unsigned int charid;
	unsigned int worldid;

	RoseSkeletonLoader* skelfemale;
	RoseSkeletonLoader* skelmale;
	RoseMeshList*  charml[ 2 ][ 16 ];
};

extern IkarusData* IDInstance;
#define IKDATA IDInstance