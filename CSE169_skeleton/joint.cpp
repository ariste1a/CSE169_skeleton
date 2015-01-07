#include "joint.h"
#include <iostream>
//TODO: implement default ctor and finish parser
// CONVERT EVERYTHING TO VECTOR3 NOT VECTOR
joint::joint()
{
}

bool joint::load(Tokenizer &tokenizer)
{
	char name[256];
	tokenizer.GetToken(name); 
	std::cout << name << std::endl;
	this->name = name; 
	this->world = new Matrix34(); 
	this->world->Identity(); 
	tokenizer.FindToken("{");
	while (1) {
		char temp[256];
		tokenizer.GetToken(temp);
		if (strcmp(temp, "offset") == 0) {			
			offset[0] = tokenizer.GetFloat();
			offset[1] = tokenizer.GetFloat();
			offset[2] = tokenizer.GetFloat();
			/*
			std::cout << offset[0] << std::endl; 
			std::cout << offset[1] << std::endl;
			std::cout << offset[2] << std::endl;
			std::cout << std::endl; */ 
		}				
		if (strcmp(temp, "boxmin") == 0) {
			boxmin[0] = tokenizer.GetFloat();
			boxmin[1] = tokenizer.GetFloat();
			boxmin[2] = tokenizer.GetFloat();
		}
		if (strcmp(temp, "boxmax") == 0) {
			boxmax[0] = tokenizer.GetFloat();
			boxmax[1] = tokenizer.GetFloat();
			boxmax[2] = tokenizer.GetFloat();
		}
		if (strcmp(temp, "rotxlimit") == 0) {
			rotxlimit.setMinMax(tokenizer.GetFloat(), tokenizer.GetFloat());
		}
		if (strcmp(temp, "rotylimit") == 0) {
			rotylimit.setMinMax(tokenizer.GetFloat(), tokenizer.GetFloat());
		}
		if (strcmp(temp, "rotzlimit") == 0) {
			rotzlimit.setMinMax(tokenizer.GetFloat(), tokenizer.GetFloat());
		}
		if (strcmp(temp, "pose") == 0) {
			pose[0] = tokenizer.GetFloat(); 
			pose[1] = tokenizer.GetFloat();
			pose[2] = tokenizer.GetFloat();
		}
		if (strcmp(temp, "balljoint") == 0) {
					joint *jnt = new joint;					
					jnt->load(tokenizer);
					addChild(jnt);
					jnt->setParent(this); 
		}
		if (strcmp(temp, "}") == 0)
		{

			return true;
		}
		else 
			tokenizer.SkipLine(); // Unrecognized token
	}
}


void joint::printChildren()
{
	//std::cout << this->getName() << std::endl;
	for (int i = 0; i < this->children.size(); i++)
	{
		std::cout << children[i]->getName() << std::endl; 
		children[i]->printChildren(); 
	}
}
void joint::addChild(joint* child)
{ 
	this->children.push_back(child); 
}

//update each world matrix first, then call draw cursively 
void joint::draw()
{
	//need to recursively call and define shit. 
	//need to redo the local matrix and multiply it by the world matrix (World * Local); 
	glMatrixMode(GL_MODELVIEW);
	//only want to make this once... 
	this->local = new Matrix34(); 
	Matrix34 *trans = new Matrix34(); 
	trans->MakeTranslate(this->offset); 
	local->Dot(*local, *trans); 
	//this->getOffset());
	glLoadMatrixf(*(this->local));
	//glLoadMatrixf(*(this->world));
	glutWireCube(2);	
	std::cout << this->getName() << std::endl; 
	//
	for (int i = 0; i < this->children.size(); i++)
	{
		std::cout << this->children[i]->getName() << std::endl;
		children[i]->draw(); 		
	}
	//determine what shape
}

std::string joint::getName()
{
	return this->name;
}

joint* joint::getParent()
{
	return this->parent; 
}

void joint::setParent(joint* parent)
{
	this->parent = parent; 
}
joint::~joint()
{
}
