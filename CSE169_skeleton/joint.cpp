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
			float min = tokenizer.GetFloat(); 
			float max = tokenizer.GetFloat(); 			
			rotxlimit.setMinMax(min, max); 			
			/*rotxlimit.setMinMax(tokenizer.GetFloat(), tokenizer.GetFloat());
			std::cout << rotxlimit.getMin() << std::endl;*/
		}
		if (strcmp(temp, "rotylimit") == 0) {
			//rotylimit.setMinMax(tokenizer.GetFloat(), tokenizer.GetFloat());
			float min = tokenizer.GetFloat();
			float max = tokenizer.GetFloat();
			rotylimit.setMinMax(min, max);
		}
		if (strcmp(temp, "rotzlimit") == 0) {
			//rotzlimit.setMinMax(tokenizer.GetFloat(), tokenizer.GetFloat());
			float min = tokenizer.GetFloat();
			float max = tokenizer.GetFloat();
			rotzlimit.setMinMax(min, max);
		}
		//degrees for each axis right?
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

void joint::draw()
{
	glLoadMatrixf(*(this->world));
	drawWireBox(boxmin.x, boxmin.y, boxmin.z, boxmax.x, boxmax.y, boxmax.z);
	for (int i = 0; i < this->children.size(); i++)
	{
		children[i]->draw();
	}
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


void joint::ComputeWorldMatrix(Matrix34 *parentMtx) {
	this->local = &ComputeLocalMatrix();
	this->world->Dot(*parentMtx,*local);
	for (int i = 0; i < this->children.size(); i++)
	{		
		children[i]->ComputeWorldMatrix(this->world);
	}
}

Matrix34 joint::ComputeLocalMatrix()
{
	//local: scale -> rotate -> translate
	this->local = new Matrix34();
	local->Identity(); 

	Matrix34 *trans = new Matrix34();
	trans->MakeTranslate(this->offset);	

	//clamping?
	//these are in radians, need to wrap around and divide by 2pi
	//messed up because of this?
	Matrix34 *rotation = new Matrix34(); 
	
	if (pose.x > rotxlimit.getMax())
	{
		pose.x = rotxlimit.getMax();
	}
	else if (pose.x < rotxlimit.getMin())
	{
		
		pose.x = rotxlimit.getMin();
	}

	if (pose.y > rotylimit.getMax())
	{
		pose.y = rotylimit.getMax();
	}
	else if (pose.y < rotylimit.getMin())
	{
		pose.y = rotylimit.getMin();
	}

	if (pose.z > rotzlimit.getMax())
	{
		pose.z = rotzlimit.getMax();
	}
	else if (pose.z < rotzlimit.getMin())
	{
		pose.z = rotzlimit.getMin();
	}
	
	//how to create rotations from 3x 1-DOFs? how to multiply them?
	rotation->FromEulers(pose.x, pose.y, pose.z, 0	);
		
	local->Dot(*trans, *rotation);
	//local->Dot(*rotation, *trans); 
	return *local;
}
joint::~joint()
{

}
