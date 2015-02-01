#include "skin.h"


skin::skin()
{	
}

bool skin::loadMorph(const char * file)
{
	Tokenizer token = *(new Tokenizer());
	token.Open(file);
	char name[256];
	token.FindToken("position");
	token.GetChar();
	int positions = token.GetInt();
	token.FindToken("{");
	for (int i = 0; i < positions; i++)
	{
		int index = token.GetInt();
		float x = token.GetFloat();
		float y = token.GetFloat();
		float z = token.GetFloat();
		this->positions[index]->x = x;
		this->positions[index]->x = y;
		this->positions[index]->x = z;
	}

	token.FindToken("normals");
	positions = token.GetInt();
	token.FindToken("{");
	for (int i = 0; i < positions; i++)
	{
		int index = token.GetInt();
		float x = token.GetFloat();
		float y = token.GetFloat();
		float z = token.GetFloat();
		this->normals[index]->x = x;
		this->normals[index]->x = y;
		this->normals[index]->x = z;
	}
	return true;
}
bool skin::load(const char *file)
{
	Tokenizer token = *(new Tokenizer());
	token.Open(file);
	char name[256];
	token.FindToken("position");
	token.GetChar();
	int positions = token.GetInt();
	token.FindToken("{");
	for (int i = 0; i < positions; i++)
	{
		float x = token.GetFloat(); 
		float y = token.GetFloat();
		float z = token.GetFloat();
		this->positions.push_back(new Vector3(x, y, z)); 
		this->posPrime.push_back(new Vector3(x, y, z));
	}
	
	token.FindToken("normals");
	positions = token.GetInt(); 
	token.FindToken("{");
	for (int i = 0; i < positions; i++)
	{
		float x = token.GetFloat();
		float y = token.GetFloat();
		float z = token.GetFloat();
		this->normals.push_back(new Vector3(x, y, z));
		this->normalsPrime.push_back(new Vector3(x, y, z));
	}
	
	//if the file contain texture coordinates. 
	if (token.FindToken("texcoords") == true){
		positions = token.GetInt();
		token.FindToken("{");
		for (int i = 0; i < positions; i++)
		{
			float x = token.GetFloat();
			float y = token.GetFloat();
			float z = 0; 
			this->texcoords.push_back(new Vector3(x, y, z));			
		}
	}
	else{
		token.Reset();
	}


	token.FindToken("skinweights");
	positions = token.GetInt();
	token.FindToken("{");
	for (int i = 0; i < positions; i++)
	{
		int numAttachments = token.GetFloat();
		skinweight *skinWeight = new skinweight(); 
		skinWeight->numAttachments = numAttachments; 
		for (int i = 0; i < numAttachments; i++)
		{
			int jointNum = token.GetInt(); 
			float weightVal = token.GetFloat(); 
			skinWeight->jointWeightPair.push_back(new std::pair<int, float>(jointNum, weightVal));
		}		
		this->skinWeights.push_back(skinWeight);

	}
	
	if (token.FindToken("material") == true){
		token.FindToken("{");
		char name[256];
		token.GetToken(name);
		token.GetToken(name);
		this->texName = name;
		std::cout << "TEXTURE NAME " << texName << std::endl;
	}
	else{
		token.Reset();
	}

	token.FindToken("triangles");
	positions = token.GetInt();
	token.FindToken("{");
	for (int i = 0; i < positions; i++)
	{
		float x = token.GetFloat();
		float y = token.GetFloat();
		float z = token.GetFloat();
		this->triangles.push_back(new Vector3(x, y, z));
	}
	 
	token.FindToken("bindings");
	positions = token.GetInt();
	token.FindToken("{");
	for (int i = 0; i < positions; i++)
	{
		token.FindToken("matrix");
		token.FindToken("{");
		float ax = token.GetFloat();
		float bx = token.GetFloat();
		float cx = token.GetFloat();
		float ay = token.GetFloat();
		float by = token.GetFloat();
		float cy = token.GetFloat();
		float az = token.GetFloat();
		float bz = token.GetFloat();
		float cz = token.GetFloat();
		float dx = token.GetFloat();
		float dy = token.GetFloat();
		float dz = token.GetFloat();
		//may or may not give the last row?
		Matrix34 *matrix = new Matrix34(ax, bx, cx, dx,
			ay, by, cy, dy,
			az, bz, cz, dz);
		matrix->FastInverse();
		this->bindings.push_back(matrix);
		//might be row major
	}
	token.Close();
	return true; 
}

//separate this method into updating the stored vertices vs drawing the actual triangles. 
//change this into just draw
void skin::draw()
{
	//wtf are you supposed to use the triangles for?
	//use each vertex as a vertex for the triangles. 
	//glLoadIdentity();
	GLfloat cyan[] = { 0.f, .8f, .8f, 1.f };
	GLfloat white[] = { 0.8f, 0.8f, 0.8f, 1.0f };
	GLfloat shiny[] = { 50.f };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, cyan);
	glMaterialfv(GL_FRONT, GL_SPECULAR, white);
	glMaterialfv(GL_FRONT, GL_SHININESS, shiny);

	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glBindTexture(GL_TEXTURE_2D, this->textureLoader.texture[0]);
	glBegin(GL_TRIANGLES);	
	//these are the positions....not the actual triangles....
	//and need the normals
	for (int i = 0; i < this->triangles.size(); i++)
	{
		Vector3 triangle = *triangles[i];

		Vector3 tri = *posPrime[triangle.x];
		Vector3 tri1 = *posPrime[triangle.y];
		Vector3 tri2 = *posPrime[triangle.z];
		
		Vector3 norm = *normalsPrime[triangle.x];
		Vector3 norm1 = *normalsPrime[triangle.y];
		Vector3 norm2 = *normalsPrime[triangle.z];
		
	
		Vector3 tex;
		Vector3 tex1; 
		Vector3 tex2;

		if (texcoords.size() > 0)
		{
			tex = *texcoords[triangle.x];
			tex1 = *texcoords[triangle.y];
			tex2 = *texcoords[triangle.z];
		}

		//if		

		glTexCoord2f(tex.x, tex.y);
		glNormal3f(norm.x, norm.y, norm.z);
		glVertex3f(tri.x, tri.y, tri.z); 
		
		glTexCoord2f(tex1.x, tex1.y);
		glNormal3f(norm1.x, norm1.y, norm1.z);
		glVertex3f(tri1.x, tri1.y, tri1.z);
		
		glTexCoord2f(tex2.x, tex2.y);
		glNormal3f(norm2.x, norm2.y, norm2.z);
		glVertex3f(tri2.x, tri2.y, tri2.z);
		
		//......each of the triangle x y z corresponds to the position of each vert.....ie. 0 1 2, vert 0, vert 1, vert 2......
		//call pose after. 	
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);

}

//change this into update
void skin::update(Skeleton* skel)
{
	this->skel = skel; 
	//foreach joint claculate M= W*B^-1....B is already inversed during parsing
	//each joint's # should correspond to the binding matrix order
	std::vector<Matrix34*> newMatrices = *new std::vector<Matrix34*>();
	for (int i = 0; i < skel->joints.size(); i++)
	{
		Matrix34 *newMtx = new Matrix34(); 
		newMtx->Dot(*skel->joints[i]->getWorldMatrix(), *bindings[i]);
		//newMtx->Dot(*bindings[i], *skel->joints[i]->getWorldMatrix());
		newMatrices.push_back(newMtx);
	}

	//need to fix updating the same set of position vectors will blow it up. need to only do the update "once" 
	for (int i = 0; i < positions.size(); i++)
	{
		Vector3 *tempVector = new Vector3(); 
		Vector3 *tempNormal = new Vector3(); 
		skinweight *currWeight = this->skinWeights[i];	
		for (int j = 0; j < currWeight->numAttachments; j++)
		{
			int currJoint = currWeight->jointWeightPair[j]->first;
			float currWeightVal = currWeight->jointWeightPair[j]->second; 
					
			Vector3 *newVec = new Vector3();
			newMatrices[currJoint]->Transform((*positions[i]), *newVec); //WiMi			
			*newVec = currWeightVal* (*newVec);
			tempVector = &(*tempVector + *newVec);
			//tempVector = tempVector + (currWeightVal)* *(positions[i]) * newMatrices[i]; 
			delete newVec; 

			//normals 
			
			newVec = new Vector3(); 
			newMatrices[currJoint]->Transform3x3((*(normals)[i]), *newVec); //WiMi			
			*newVec = currWeightVal* (*newVec);
			tempNormal = &(*tempVector + *newVec);
			delete newVec;  
		}
		  
		//delete;  tempVector
		*(posPrime[i]) = *tempVector;	
		tempNormal->Normalize(); 
		*(normalsPrime[i]) = *tempNormal;
		//	delete tempNormal;
	}
	for (auto it = newMatrices.begin(); it != newMatrices.end(); ++it){
		delete *it;
	}
	for (auto it = positions.begin(); it != positions.end(); ++it){
		delete *it;
	}
	positions.clear();
	newMatrices.clear(); 
	//delete &newMatrices;
	
	//v' = weight1(Matrix1 *v) 	
	//Loop through vertices and compute blended position & normal

}


/*Set GL matrix state to Identity (world)
Loop through triangles and draw using world space positions &
normals
*/

skin::~skin()
{
	delete &positions;
	delete &posPrime;
	delete &normals;
	delete &skinWeights;
	delete &triangles;
	delete &bindings;
}