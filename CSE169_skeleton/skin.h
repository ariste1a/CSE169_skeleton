#pragma once
#include "../token.h"
#include "../vector3.h"
#include <vector>
#include "../matrix34.h"
#include "Skeleton.h"
class skin
{
public:
	skin();
	bool load(const char *file);
	bool load(Tokenizer &t);
	std::vector<Vector3*> positions; 
	std::vector<Vector3*> posPrime;
	std::vector<Vector3*> normals;
	std::vector<Vector3*> skinWeights;
	std::vector<Vector3*> triangles;
	std::vector<Matrix34*> bindings;	
	Skeleton *skel; 
	void bind(Skeleton* skel);
	void skin::draw();
	void skin::update(Skeleton* skel);
	~skin();
};

