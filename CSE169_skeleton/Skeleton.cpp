#include "Skeleton.h"


Skeleton::Skeleton()
{
}

bool Skeleton::load(const char *file)
{
	Tokenizer token = *(new Tokenizer()); 
	token.Open(file);
	token.FindToken("balljoint");
	// Parse tree
	joint* root = new joint(); 
	this->setRoot(root); 
	this->getRoot()->load(token);
	// Finish
	token.Close();
	return true;
}

joint* Skeleton::getRoot()
{
	return this->skelRoot; 
}

void Skeleton::setRoot(joint* root)
{
	this->skelRoot = root; 
}

void Skeleton::draw()
{
	this->getRoot()->draw(); 
}

void Skeleton::update()
{	
	//this->skelRoot->ComputeWorldMatrix(local);
	this->skelRoot->computeWorldWithPose(local);
}
Skeleton::~Skeleton()
{
}