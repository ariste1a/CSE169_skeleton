#pragma once
#include "../matrix34.h"
#include "DOF.h"
#include <vector>
#include "../token.h"
#include <string>
#include "../vector3.h"
#include "../core.h"
#include <cmath>
class joint
{
private: 	
	Matrix34 *local; 
	Matrix34 *world;
	std::vector<DOF*> dofs;
	joint *parent;	
	std::vector<joint*> children; 
	std::string name; 	
	Vector3 offset = *(new Vector3(0, 0, 0));	
	Vector3 boxmin = *(new Vector3(-0.1, -0.1, -0.1));
	Vector3 boxmax = *(new Vector3(0.1, 0.1, 0.1));
	DOF rotxlimit;
	DOF rotylimit;
	DOF rotzlimit;
	Vector3 pose = *(new Vector3(0,0,0));	
	
public:		
	joint();
	void init(); 
	bool load(Tokenizer &t);
	void update();
	void draw();
	void reset();
	void addChild(joint* child); 
	std::string getName(); 
	void setName(std::string); 	
	std::vector<float> getOffset();
	void setOffset(std::vector<float>);
	void setParent(joint* parent); 
	joint* getParent(); 
	void printChildren();
	void drawBone(); 
	void ComputeWorldMatrix(Matrix34 *parentMatrix);
	Matrix34 ComputeLocalMatrix();
	//if want to 
	//virtual void MakeLocalMatrix();
	~joint();
};

