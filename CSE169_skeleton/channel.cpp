#include "channel.h"


channel::channel()
{	
	hermite = new Matrix34(2.0, -2.0, 1.0, 1.0, 
						  -3.0, 3, - 2, -1,
						   0, 0, 1, 0, 
						   1, 0, 0, 0);
}


//if t is before first key use extrapolation 
//if t is after the last key use extrapolation
//if t falls exactly on a key return the key value
//if t falls between two keys evaluat the cubic equation. 


//sequential search
float channel::evaluate(float time)
{
	float t0; 
	float t1; 
	int i = 0;
	if (time < (*keyFrames)[0]->time)
	{
		//std::cout << "extrapolate" << std::endl; 
		//extrapolate
	}
	if (time < (*keyFrames).back()->time)
	{
		//std::cout << "extrapolate" << std::endl;
		//extrapolate
	}
	for (i; i < keyFrames->size(); i++)
	{
		if ((*keyFrames)[i]->time == time)
		{						
			return (*keyFrames)[i]->value;
		}
		if ((*keyFrames).size() > i+1)
		{
			if (time <(*keyFrames)[i]->time && time > (*keyFrames)[i + 1]->time)
			{
				t0 = (*keyFrames)[i]->time;
				t1 = (*keyFrames)[i+1]->time;
				(*keyFrames)[i]->inverseLerp(time);
				return (*keyFrames)[i]->evalSpan(time);				
			}
		}
	}

	if (i == keyFrames->size())
	{
		i -= 1; 
	}

	(*keyFrames)[i]->inverseLerp(time);
	return (*keyFrames)[i]->evalSpan(time);
}

channel::~channel()
{

}

void channel::precomputeCubics()
{
	
	for (int i = 0; i < keyFrames->size(); i++)
	{
		float vin = 0;
		float vout = 0;
		float p0;
		float p1;
		float t0;
		float t1;		

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

					(*keyFrames)[0]->precomSpan = 1 / (t0-t1);
				}
				else if (keyFrames->size() == 1)
				{
					(*keyFrames)[i]->out = 0;
					(*keyFrames)[i]->in = 0;					
				}
			}	
			//need to program if it's the last one
			else if (i == keyFrames->size()-1)
			{
				p0 = (*keyFrames)[i - 1]->value;
				p1 = (*keyFrames)[i]->value;

				t0 = (*keyFrames)[i-1]->time;
				t1 = (*keyFrames)[i]->time;
			}
			else
			{
				p1 = (*keyFrames)[i+1]->value;
				p0 = (*keyFrames)[i - 1]->value;

				t0 = (*keyFrames)[i-1]->time;
				t1 = (*keyFrames)[i+1]->time;
			}			
			vin = (p1 - p0) / (t1 - t0);

			(*keyFrames)[i]->out = vin;
			(*keyFrames)[i]->in = vin;
		}
		else if ((*keyFrames)[i]->tanIn == "flat")
		{
			(*keyFrames)[i]->in = 0;
			(*keyFrames)[i]->out = 0;
		}
		else if ((*keyFrames)[i]->tanIn == "linear")
		{
			if (i == 0)
			{
				if (keyFrames->size() > 1)
				{
					p0 = (*keyFrames)[0]->value;
					p1 = (*keyFrames)[1]->value;

					t0 = (*keyFrames)[0]->time;
					t1 = (*keyFrames)[1]->time;
					vin = (p1 - p0) / (t1 - t0);
					vout = vin;

					(*keyFrames)[0]->out = vin; 
					(*keyFrames)[1]->in = vin;
				}
			}
			//need to program if it's the last one
			else if (i == keyFrames->size() - 1)
			{
				(*keyFrames)[i]->out = (*keyFrames)[i]->in;
			}
			else
			{
				p0 = (*keyFrames)[i]->value;
				p1 = (*keyFrames)[i+1]->value;

				t0 = (*keyFrames)[i]->time;
				t1 = (*keyFrames)[i+1]->time;
				vin = (p1 - p0) / (t1 - t0);

				(*keyFrames)[i]->out = vin;
				(*keyFrames)[1+1]->in = vin;
			}
		}		
		(*keyFrames)[i]->precomSpan = 1 / (t0 - t1);
	}

	for (int i = 0; i < keyFrames->size(); i++)
	{
		float p0 = (*keyFrames)[i]->value; 
		float p1 = (*keyFrames)[i+1]->value;

		float t0 = (*keyFrames)[i]->time;
		float t1 = (*keyFrames)[i]->time;

		float v0 = (*keyFrames)[i]->in;
		float v1 = (*keyFrames)[i]->out;

		(*keyFrames)[i]->precomputeCoeff(*this->hermite, p0, p1, t0, t1, v0, v1);
	}
}