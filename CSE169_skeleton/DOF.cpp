#define _USE_MATH_DEFINES
#include "DOF.h"
#include <cmath>
//TODO: Implement setters and getters and ctor for default values. 

DOF::DOF()
{
	value = 0; 
	min = -100000;
	max = 100000;
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
	//this->min = clampRad(min);
	this->min = min; 
}
void DOF::setMax(float max)
{
	//this->max = clampRad(max);
	this->max = max; 

}
void DOF::setMinMax(float min, float max)
{
	this->min = min;
	this->max = max; 
}
/*
startAngle = fmod(startAngle, 360.0);
if(startAngle < 0)
startAngle += 360.0;
*/
double DOF::clampRad(double rad)
{
	if (rad > M_PI)
	{
		rad -= floorf(rad / M_PI) * M_PI;
	}
	while (rad < 0)
	{
		rad += M_PI;
	}
	return rad;
}
DOF::~DOF()
{
}
