#include "key.h"


key::key()
{
	cubicCoeff = new Vector3(); 
}

void key::precomputeCoeff(Matrix34 hermite, float p0, float p1, float t0, float t1, float v0, float v1)
{
	Vector3 params = *new Vector3(p0, p1, v0, v1);
	Vector3 res = *new Vector3(); 
	hermite.VectorCross(params, *cubicCoeff); 
}

void key::inverseLerp(float t)
{
	//lerpTime = (t - t0) / (t1 - t0); 
	lerpTime = t * precomSpan;
}

float key::evalSpan(float t)
{
	float a = cubicCoeff->x;
	float b = cubicCoeff->y;
	float c = cubicCoeff->z;
	float d = cubicCoeff->w;
	float u = lerpTime; 
	float x; 
		
	x = (a*u*u*u) + (b*u*u) + (c*u) + d;
	return x; 
}

key::~key()
{
}
