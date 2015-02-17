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
}

void animation::animate()
{
	//use the range of the file to set how many times to repeat. 
}

animation::~animation()
{

}
