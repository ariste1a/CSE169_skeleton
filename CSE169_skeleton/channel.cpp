#include "channel.h"


channel::channel()
{	
	
	hermite = new Matrix34(2.0, -2.0, 1.0, 1.0, 
						  -3.0, 3, - 2, -1,
						   0, 0, 1, 0, 
						   1, 0, 0, 0);
	cycleNum = 0; 
	//hermite = new Matrix34()
	
	/*
	hermite = new Matrix34(2, -3, 0, 1,
							-2, 3, 1, 0,
							1, -2, 1, 0,
							1, -1, 0, 0);
				*/			
	float cycleDistance = 0;
	bool cycleReset = true;
	float cycleTime = 0;
}


//if t is before first key use extrapolation 
//if t is after the last key use extrapolation
//if t falls exactly on a key return the key value
//if t falls between two keys evaluat the cubic equation. 

//sequential search
float channel::evaluate(float time)
{
	
	if (time < (*keyFrames)[0]->time)
	{
		if (this->extrapolate1 == "constant")
		{
			return keyFrames->at(0)->value; 
		}
		if (this->extrapolate1 == "cycle_offset")
		{
			//use the previous last value;
			cycleNum = time / keyFrames->back()->time;
			float mod = keyFrames->back()->time - keyFrames->at(0)->time;
			time = fmod(time, mod) + (*keyFrames)[0]->time;
		}
		if (this->extrapolate1 == "cycle")
		{
			//restart from the beginning; 			
			//float mod = keyFrames->back()->time - keyFrames->at(0)->time;
			while (time < keyFrames->at(0)->time)
			{
				time += (keyFrames->back()->time - keyFrames->at(0)->time);
				//value -= keyFrames->back()->value - keyFrames->at(0)->value; 
			}
			//time = fmod(time, mod) + (*keyFrames)[0]->time;
		}
	}

	if (time > keyFrames->back()->time)
	{
		//std::cout << "extrapolate" << std::endl;
		//extrapolate
		if (this->extrapolate2 == "constant")
		{
			return keyFrames->back()->value;
		}
		if (this->extrapolate2 == "cycle_offset")
		{
			//divide to get curr # cycles
			//std::cout << time / keyFrames->back()->time << std::endl;
			cycleNum = time / keyFrames->back()-> time; 
			float mod = keyFrames->back()->time - keyFrames->at(0)->time;
			time = fmod(time, mod) + (*keyFrames)[0]->time;						
		}
		if (this->extrapolate2 == "cycle")
		{
			//float mod = keyFrames->back()->time - keyFrames->at(0)->time;
			//time = fmod(time, mod) + keyFrames->at(0)->time;
			while (time > keyFrames->back()->time)
			{
				time -= (keyFrames->back()->time - keyFrames->at(0)->time);
				//value -= keyFrames->back()->value - keyFrames->at(0)->value; 
			}
		}
	}	

	if (keyFrames->size() == 1)
	{
		//return keyFrames->at(0)->evalSpan(time); 
		return keyFrames->at(0)->value;
	}
	for (int i = 0; i < keyFrames->size(); i++)
	{
			if ((*keyFrames)[i]->time == time)
			{
				return (*keyFrames)[i]->value;
			}			
			/*
			if ((*keyFrames)[i]->time < time && time < (*keyFrames)[i + 1]->time)
			{
				if (i < (*keyFrames).size() - 1)
				{
					float t0 = (*keyFrames)[i]->time;
					float t1 = (*keyFrames)[i + 1]->time;
					(*keyFrames)[i]->inverseLerp(t0, time, t1);
					return (*keyFrames)[i]->evalSpan(time);
				}
			}
			*/			
			if (time < keyFrames->at(i)->time)
			{
				float t1 = (*keyFrames)[i]->time;
				float t0 = (*keyFrames)[i-1]->time;
				(*keyFrames)[i-1]->inverseLerp(t0, time, t1);
				return (*keyFrames)[i-1]->evalSpan(time);
			}
		//return (*keyFrames)[i]->evalSpan(time);
	}
}

channel::~channel()
{

}

void channel::precomputeCubics()
{
	
	//calculate tangeants 
	for (int i = 0; i < keyFrames->size(); i++)
	{
		float vin = 0;
		float vout = 0;
		float p0 = 0; 
		float p1 = 0; 
		float t0 = 0;
		float t1 = 0; 

		if ((*keyFrames)[i]->tanIn == "flat")
		{
			(*keyFrames)[i]->in = 0;
			(*keyFrames)[i]->out = 0;
			continue;
		}

		if ((*keyFrames)[i]->tanIn == "smooth")
		{
			//if first smooth, then apply linear
			if (i == 0)
			{
				if (keyFrames->size() > 1)
				{
					p0 = (*keyFrames)[0]->value;
					p1 = (*keyFrames)[1]->value;

					t0 = (*keyFrames)[0]->time;
					t1 = (*keyFrames)[1]->time;					
				}
				else if (keyFrames->size() == 1)
				{
					(*keyFrames)[i]->out = 0;
					(*keyFrames)[i]->in = 0;
					continue;
				}
			}	 
			//need to program if it's the last one
			else if (i == keyFrames->size()-1)
			{
				p0 = (*keyFrames)[i-1]->value;
				p1 = (*keyFrames)[i]->value;

				t0 = (*keyFrames)[i-1]->time;
				t1 = (*keyFrames)[i]->time;
				vin = (p1 - p0) / (t1 - t0);
				if ((t1 - t0) == 0)
				{
					vin = 0;
				}
				(*keyFrames)[i]->out = vin;
				keyFrames->at(0)->in = vin; 
				continue;
			}
			else
			{
				p0 = (*keyFrames)[i - 1]->value;
				p1 = (*keyFrames)[i+1]->value;

				t0 = (*keyFrames)[i-1]->time;
				t1 = (*keyFrames)[i+1]->time;
			}
			vin = (p1 - p0) / (t1 - t0);
			if ((t1 - t0) == 0)
			{
				vin = 0;
			}
			(*keyFrames)[i]->out = vin;
			(*keyFrames)[i]->in = vin;
			continue;
		}	   
		
		if ((*keyFrames)[i]->tanIn == "linear")
		{
			if (i == keyFrames->size() - 1)
			{
				p0 = keyFrames->at(i-1)->value;
				p1 = (*keyFrames)[i]->value;

				t0 = (*keyFrames)[i-1]->time;
				t1 = (*keyFrames)[i]->time;
				vin = (p1 - p0) / (t1 - t0);
				if ((t1 - t0) == 0)
				{
					vin = 0;
				}
				vout = vin;

				(*keyFrames)[i-1]->out = vin;
				(*keyFrames)[i]->in = vin;
				continue; 
			}
			else if(i  < keyFrames->size())
			{
				p0 = (*keyFrames)[i]->value;
				p1 = (*keyFrames)[i+1]->value;

				t0 = (*keyFrames)[i]->time;
				t1 = (*keyFrames)[1+1]->time;
				vin = (p1 - p0) / (t1 - t0);
				if ((t1 - t0) == 0)
				{
					vin = 0;
				}
				vout = vin;

				(*keyFrames)[i]->out = vin;
				(*keyFrames)[1+1]->in = vin;
				continue;
			}			
		}
	}
	
	for (int i = 0; i < keyFrames->size()-1; i++)
	{
		float p0 = (*keyFrames)[i]->value; 
		float p1 = (*keyFrames)[i+1]->value;

		float t0 = (*keyFrames)[i]->time;
		float t1 = (*keyFrames)[i+1]->time;

		float v0 = (*keyFrames)[i]->out;
		float v1 = (*keyFrames)[i+1]->in;

		(*keyFrames)[i]->precomputeCoeff(*this->hermite, p0, p1, t0, t1, v0, v1);
	}
	
	if (keyFrames->size() == 1)
	{
		float p0 = (*keyFrames)[0]->value;
		float p1 = (*keyFrames)[0]->value;

		float t0 = (*keyFrames)[0]->time;
		float t1 = (*keyFrames)[0]->time;

		float v0 = (*keyFrames)[0]->in;
		float v1 = (*keyFrames)[0]->out;

		(*keyFrames)[0]->precomputeCoeff(*this->hermite, p0, p1, t0, t1, v0, v1);
	}	
}