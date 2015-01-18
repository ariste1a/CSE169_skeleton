#pragma once
#include "joint.h"
#include "DOF.h"
#include "../token.h"
#include <vector> 
#include "Offset.h"
#include "pose.h"
class Skeleton
{
private: 
	joint * skelRoot; 
	pose* pose; 
	Matrix34 *local = new Matrix34(); 
public:
	Skeleton();
	void init(); 
	bool load(const char *file); 
	void update(); 
	void draw(); 
	void reset(); 
	joint* getRoot();
	void setRoot(joint*); 
	Offset offset; 
	std::vector<joint*> joints; 
	~Skeleton();
};

