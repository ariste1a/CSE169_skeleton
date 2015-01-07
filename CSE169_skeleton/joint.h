#pragma once
#include "../matrix34.h"
#include "DOF.h"
#include <vector>
#include "../token.h"
#include <string>
class joint
{
private: 	
	Matrix34 *local; 
	Matrix34 *world;
	std::vector<DOF*> dofs;
	joint *parent;	
	std::vector<joint*> children; 
	std::string name; 
	std::vector<float> offset = *(new std::vector<float>(3, 0.0)); 
	std::vector<float> boxmin = *(new std::vector<float>(3, -0.1)); 
	std::vector<float> boxmax = *(new std::vector<float>(3, 0.1));
	DOF rotxlimit;
	DOF rotylimit;
	DOF rotzlimit;
	std::vector<float> pose = *(new std::vector<float>(3,0.0));
	
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
	//if want to 
	//virtual void MakeLocalMatrix();
	~joint();
};

