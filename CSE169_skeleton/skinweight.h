#pragma once
#include <utility>
#include <vector>
class skinweight
{
public:
	int numAttachments; 
	std::vector<std::pair<int, float>*> jointWeightPair;
	skinweight();
	~skinweight();
};

