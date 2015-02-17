#pragma once
#include "../core.h"
#include <vector>
#include <utility>
#include "channel.h"
#include "Skeleton.h"
#include "channel.h"

class animation
{
public:
	animation();
	std::pair<float, float>* range; 
	int numChannels;
	std::vector<channel*>* channels;
	void animate(float time); 
	Skeleton* skeleton; 
	void load(char* filename);	
	Skeleton skel; 
	~animation();
};

