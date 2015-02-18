#pragma once
#include "../core.h"
#include "key.h"
#include "../matrix34.h"
class channel
{
public:
	channel();
	std::string extrapolate1; 
	std::string extrapolate2;
	std::vector<key*> *keyFrames = new std::vector<key*>();
	float evaluate(float time); //find the proper span for the given time, then evaluate the cubic equation for the span. 
	void precomputeCubics();
	key findKeyframe(); 
	Matrix34 *hermite;
	int cycleNum; 
	float value;
	~channel();
};