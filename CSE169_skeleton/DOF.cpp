#include "DOF.h"

//TODO: Implement setters and getters and ctor for default values. 

DOF::DOF()
{
	value = 0; 
	min = 0; 
	max = 0; 
}

float DOF::getValue()
{
	return this->value;
}

float DOF::getMin()
{
	return this->min; 
}
float DOF::getMax()
{
	return this->max; 
}
void DOF::setMin(float min)
{
	this->min = min; 
}
void DOF::setMax(float max)
{
	this->max = max; 
}
void DOF::setMinMax(float min, float max)
{
	this->min = min; 
	this->max = max; 
}

DOF::~DOF()
{
}
