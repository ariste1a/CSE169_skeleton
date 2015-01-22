#include "skin.h"


skin::skin()
{	
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
	}

	/* NEED TO REDO*/
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
		matrix->Inverse();
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
	//need to attach all of these to their respective joints... 
	//for each joint draw this thingy? or no?


	/*glBegin(GL_TRIANGLES);
	glColor3f(0.1, 0.2, 0.3);
	glVertex3f(0, 0, 0);
	glVertex3f(1, 0, 0);
	glVertex3f(0, 1, 100);
	glEnd();
	*/
	//wtf are you supposed to use the triangles for?
	//use each vertex as a vertex for the triangles. 
	glBegin(GL_TRIANGLES);
	//these are the positions....not the actual triangles....
	//and need the normals
	for (int i = 0; i < this->triangles.size(); i++)
	{
		Vector3 triangle = *triangles[i];

		Vector3 tri = *posPrime[triangle.x];
		Vector3 tri1 = *posPrime[triangle.y];
		Vector3 tri2 = *posPrime[triangle.z];
		  
		Vector3 norm = *normals[triangle.x];
		Vector3 norm1 = *normals[triangle.y];
		Vector3 norm2 = *normals[triangle.z];

		glColor3f(1.f, 0.f, 0.f);
		glNormal3f(norm.x, norm.y, norm.z);
		glVertex3f(tri.x, tri.y, tri.z); 
		glNormal3f(norm1.x, norm1.y, norm1.z);
		glVertex3f(tri1.x, tri1.y, tri1.z);
		glNormal3f(norm2.x, norm2.y, norm2.z);
		glVertex3f(tri2.x, tri2.y, tri2.z);

		//......each of the triangle x y z corresponds to the position of each vert.....ie. 0 1 2, vert 0, vert 1, vert 2......
		//call pose after. 	
	}
	glEnd();
	
}

//change this into update
void skin::update(Skeleton* skel)
{
	this->skel = skel; 
	//foreach joint claculate M= W*B^-1....B is already inversed during parsing
	//each joint's # should correspond to the binding matrix order
	std::vector<Matrix34*> *newMatrices = new std::vector<Matrix34*>();
	for (int i = 0; i < skel->joints.size(); i++)
	{
		Matrix34 *newMtx = new Matrix34(); 
		//newMtx->Dot(*skel->joints[i]->getWorldMatrix(), *bindings[i]);
		newMtx->Dot(*skel->joints[i]->getWorldMatrix(), *bindings[i]);
		newMatrices->push_back(newMtx);
	}

	//need to fix updating the same set of position vectors will blow it up. need to only do the update "once" 
	for (int i = 0; i < positions.size(); i++)
	{
		Vector3 *tempVector = new Vector3(); 
		Matrix34 *tempNormal = new Matrix34(); 
		skinweight *currWeight = this->skinWeights[i];	//BREAKS HERE IF UPDATES?!	
		for (int j = 0; j < currWeight->numAttachments; j++)
		{
			int currJoint = currWeight->jointWeightPair[j]->first;
			float currWeightVal = currWeight->jointWeightPair[j]->second; 

			Vector3 skinXpos = *new Vector3();
			skinXpos = currWeightVal* (*(positions)[i]); 
			
			Vector3 *newVec = new Vector3();
			(*newMatrices)[currJoint]->Transform(skinXpos, *newVec); //WiMi
			tempVector = &(*tempVector + *newVec);

			//tempVector = tempVector + (currWeightVal)* *(positions[i]) * newMatrices[i]; 
		}
		//posPrime[i] = tempVector; //now converging to 0... 
		*(posPrime[i]) = *tempVector;
	
	}
	//v' = weight1(Matrix1 *v) 
	// where Matrix1 = jointWorld1 * Binding1.inverse()
	//Loop through vertices and compute blended position & normal

}


/*Set GL matrix state to Identity (world)
Loop through triangles and draw using world space positions &
normals
*/

skin::~skin()
{
}