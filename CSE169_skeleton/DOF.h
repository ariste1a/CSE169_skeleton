#pragma once
class DOF
{
private: 
	float value; 
	float min; 
	float max; 
	//offset object?
public:
	DOF();
	void init();
	void load();
	void update();
	void draw();
	void reset();	
	void setValue(float); 
	float getValue();
	float getMin(); 
	float getMax(); 
	void setMin(float); 
	void setMax(float); 
	void setMinMax(float min, float max);
	double clampRad(double rad);
	~DOF();
};

