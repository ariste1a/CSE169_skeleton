#include "animation.h"

//each animation object will have an array of channels, which each will have an array of keys. 
//each channel corresponds to a DOF which values change over time based on the function 

//how to do timing? use system time ms

//For interpolation, take the current keyframe, look n-1 and n+1. Take the values and interpolate according to the I/O rules. 
// for flat, take a look at the next one x
// for linear, take the two vals and use that as the function
// for smooth take n-1 and n+1, create slope and put that as n's slope. 
animation::animation()
{
	this->channels = new std::vector<channel*>();
}

/* MIGHT NEED animationPlayer CLASS?*/

void animation::load(char* filename)
{
	Tokenizer tokenizer = *new Tokenizer(); 
	tokenizer.Open(filename);
	char name[256];
	tokenizer.GetToken(name);
	//std::cout << name << std::endl;
	tokenizer.FindToken("{");
	tokenizer.FindToken("range");
	float rangeStart = tokenizer.GetFloat(); 
	float rangeEnd = tokenizer.GetFloat(); 
	tokenizer.FindToken("numchannels");
	this->numChannels = tokenizer.GetInt(); 
	range = new std::pair<float, float>(rangeStart, rangeEnd); 
	
	for (int i = 0; i < numChannels; i++)
	{
		channel *chan = new channel(); 		
		tokenizer.FindToken("channel {");
		tokenizer.FindToken("extrapolate");
		tokenizer.GetToken(name);
		chan->extrapolate1 = std::string(name); 
		tokenizer.GetToken(name);
		chan->extrapolate2 = std::string(name); 
		tokenizer.FindToken("keys");
		int numKeys = tokenizer.GetInt();
		tokenizer.FindToken("{"); 
		for (int j = 0; j < numKeys; j++)
		{
			key *k = new key(); 
			k->time = tokenizer.GetFloat(); 
			k->value = tokenizer.GetFloat();
			tokenizer.GetToken(name);
			k->tanIn = std::string(name);
			tokenizer.GetToken(name);
			k->tanOut = std::string(name);
			chan->keyFrames->push_back(k);
		}
		this->channels->push_back(chan); 
	}
	tokenizer.Close();

	for (int i = 0; i < channels->size(); i++)
	{
		channels->at(i)->precomputeCubics();
	}
}

void animation::animate(float time)
{
	//use the range of the file to set how many times to repeat.
	//first three channels are for root movement
	float trueTime = fmod(time, this->range->second);
	//trueTime = (float)(floor(trueTime*(1.0f / 4.0) + 0.5) / (1.0f / 4.0));
	//trueTime = floor(trueTime * 1000 + 0.5) / 1000;
	int numJoints = skel.joints.size(); 	

	float movX = channels->at(0)->evaluate(trueTime);
	float movY = channels->at(1)->evaluate(trueTime);
	float movZ = channels->at(2)->evaluate(trueTime);

	//skel.joints.at(0)->setOffset(movX, movY, movZ);
	std::cout << "moving " + skel.joints.at(0)->getName() + "by " << channels->at(2)->evaluate(trueTime) << " at " << trueTime <<  std::endl;
	for (int i = 3; i < this->channels->size(); i+=3)
	{
 		int currJoint = (i / 3)-1;
		float x = channels->at(i)->evaluate(trueTime);
		float y = channels->at(i + 1)->evaluate(trueTime);
		float z = channels->at(i + 2)->evaluate(trueTime);
		skel.joints.at(currJoint)->changeDOF(1, channels->at(i)->evaluate(trueTime));
		skel.joints.at(currJoint)->changeDOF(2, channels->at(i+1)->evaluate(trueTime));
		skel.joints.at(currJoint)->changeDOF(3, channels->at(i+2)->evaluate(trueTime));

		/*
		skel.joints.at(currJoint)->dofX = channels->at(i)->evaluate(trueTime);
		skel.joints.at(currJoint)->dofY = channels->at(i + 1)->evaluate(trueTime);
		skel.joints.at(currJoint)->dofZ = channels->at(i + 2)->evaluate(trueTime);
		*/
	}

	//or loop through all joints then evaluate their channel.
}

animation::~animation()
{

}
