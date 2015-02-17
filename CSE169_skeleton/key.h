#pragma once
#include "../core.h"
#include "../vector3.h"
#include "../matrix34.h"
class key
{
public:
	key();
	float time; 
	float value;
	std::string tanIn; 
	std::string tanOut; 
	Vector3 *cubicCoeff;
	void precomputeCoeff(Matrix34 hermite, float p0, float p1, float t0, float t1, float v0, float v1);
	void  inverseLerp(float t);
	float lerpTime; 
	float key::evalSpan(float t);
	float in;
	float out; 
	float precomSpan; 
	~key();
};

