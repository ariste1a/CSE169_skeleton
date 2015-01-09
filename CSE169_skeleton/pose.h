#pragma once
#include "../vector3.h"
#include "../matrix34.h"
class joint;
class pose
{
private: 
	Vector3 poseVector; 

public:
	pose();
	Vector3 getPose(); 
	void setPose(Vector3 pose);
	void doPose(joint* bone); 
	~pose();
};
