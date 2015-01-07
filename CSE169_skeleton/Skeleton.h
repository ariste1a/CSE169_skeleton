#pragma once
#include "joint.h"
#include "DOF.h"
#include "../token.h"
#include <vector> 
#include "Offset.h"
class Skeleton
{
private: 
	joint * skelRoot; 
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
	~Skeleton();
};

