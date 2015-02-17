////////////////////////////////////////
// tester.cpp
////////////////////////////////////////

#include "tester.h"
#include <iostream>
#define WINDOWTITLE	"EL PSY CONGROO"

////////////////////////////////////////////////////////////////////////////////

static Tester *TESTER;

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	TESTER = new Tester(argc,argv);
	TESTER->start = clock();
	glutMainLoop();	
	return 0;
}

////////////////////////////////////////////////////////////////////////////////

// These are really HACKS to make glut call member functions instead of static functions
static void display()									{TESTER->Draw();}
static void idle()										{TESTER->Update();}
static void resize(int x,int y)							{TESTER->Resize(x,y);}
static void keyboard(unsigned char key,int x,int y)		{TESTER->Keyboard(key,x,y);}
static void mousebutton(int btn,int state,int x,int y)	{TESTER->MouseButton(btn,state,x,y);}
static void mousemotion(int x, int y)					{TESTER->MouseMotion(x,y);}

////////////////////////////////////////////////////////////////////////////////

Tester::Tester(int argc,char **argv) {
	WinX=640;
	WinY=480;
	LeftDown=MiddleDown=RightDown=false;
	MouseX=MouseY=0;
	std::cout << argc << std::endl;
	//loading skeleton and skin
	std::cout << argv[1] << std::endl; 
	this->skeleton = *(new Skeleton()); 
	skeleton.load(argv[1]); 
	this->skin.load(argv[2]);
	skinName = *new std::string(argv[2]);
	this->skin.skel = &this->skeleton;
	//morph = *new std::string(argv[3]);
	//morph2 = *new std::string(argv[4]);
	anim = new animation(); 
	anim->load(argv[3]);
	std::cout << std::endl;	
	
	//getting all the children
	//this->skeleton.getRoot()->printChildren(); 	
	std::vector<joint*>* joints = new std::vector<joint*>; 
	joints->push_back(this->skeleton.getRoot()); 
	std::cout << this->skeleton.getRoot()->getName() << std::endl; 
	this->skeleton.getRoot()->traverse(joints);
	std::cout << joints->size() << std::endl;
	this->skeleton.joints = *joints;

	// Create the window
	glutInitDisplayMode( GLUT_RGBA| GLUT_DOUBLE | GLUT_DEPTH );
	glutInitWindowSize( WinX, WinY );
	glutInitWindowPosition( 0, 0 );
	WindowHandle = glutCreateWindow( WINDOWTITLE );
	glutSetWindowTitle( WINDOWTITLE );
	glEnable(GL_LIGHTING);
	glDisable(GL_BLEND);
	//glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);

	GLfloat light0_diffuse[] = { 1, 1.0, 0, 1.0 };
	GLfloat light1_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
	GLfloat light1_diffuse[] = { 0, 1.0, 1.0, 1.0 };	
	GLfloat light1_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light1_position[] = { -2.0, 2.0, 1.0, 1.0 };
	GLfloat spot_direction[] = { -1.0, -1.0, 0.0 };

	glLightfv(GL_LIGHT1, GL_AMBIENT, light1_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light1_specular);
	glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	GLfloat lightpos[] = { 3, 2, 5, 1 };
	glLightfv(GL_LIGHT0, GL_POSITION, lightpos);

	GLfloat lightpos2[] = { -3, 2, -5, 1 };
	glLightfv(GL_LIGHT1, GL_POSITION, lightpos2);


	glutSetWindow( WindowHandle );

	//ok works
	glEnable(GL_DEPTH_TEST);	

	// Background color
	glClearColor( 0., 0., 0., 1. );	

	// Callbacks
	glutDisplayFunc( display );
	glutIdleFunc( idle );
	glutKeyboardFunc( keyboard );
	glutMouseFunc( mousebutton );
	glutMotionFunc( mousemotion );
	glutPassiveMotionFunc( mousemotion );
	glutReshapeFunc( resize );

	// Initialize components

	Cam.SetAspect(float(WinX)/float(WinY));	
	if (this->skin.texcoords.size() >0)
	this->skin.textureLoader.LoadGLTextures(skin.texName.c_str());
}

////////////////////////////////////////////////////////////////////////////////

Tester::~Tester() {
	glFinish();
	glutDestroyWindow(WindowHandle);
}

////////////////////////////////////////////////////////////////////////////////

void Tester::Update() {
	// Update the components in the world
	Cam.Update();
	Cube.Update();
	skeleton.update();
	skin.update(&this->skeleton);
	// Tell glut to re-display the scene
	glutSetWindow(WindowHandle);
	glutPostRedisplay();
}

////////////////////////////////////////////////////////////////////////////////

void Tester::Reset() {
	Cam.Reset();
	Cam.SetAspect(float(WinX)/float(WinY));

	Cube.Reset();
}

////////////////////////////////////////////////////////////////////////////////

void Tester::Draw() {

	// Begin drawing scene
	glViewport(0, 0, WinX, WinY);
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// Draw components
	Cam.Draw();		// Sets up projection & viewing matrices
	//Cube.Draw();
	//skeleton.draw(); 
	glLoadIdentity();
	
	skin.draw();
	// Finish drawing scene
	glFinish();
	glutSwapBuffers();
}

////////////////////////////////////////////////////////////////////////////////

void Tester::Quit() {
	glFinish();
	glutDestroyWindow(WindowHandle);
	exit(0);
}

////////////////////////////////////////////////////////////////////////////////

void Tester::Resize(int x,int y) {
	WinX = x;
	WinY = y;
	Cam.SetAspect(float(WinX)/float(WinY));
}

////////////////////////////////////////////////////////////////////////////////

void Tester::Keyboard(int key,int x,int y) {
	switch(key) {
		case 0x1b:		// Escape
			Quit();
			break;  
		case 'r':
			Reset();
			break;
		case 'q':
			skeleton.selectedJoint--;
			if (skeleton.selectedJoint < 0)
			{
				skeleton.selectedJoint = skeleton.joints.size() - 1;
			}
			currJoint = skeleton.joints[skeleton.selectedJoint];
			std::cout << "SELECTED JOINT: " << currJoint->getName() << std::endl;
			break;

		case  'e':
			skeleton.selectedJoint++;
			if (skeleton.selectedJoint >= skeleton.joints.size())
			{
				skeleton.selectedJoint = 0;
			}
			currJoint = skeleton.joints[skeleton.selectedJoint];
			std::cout << "SELECTED JOINT: " << currJoint->getName() << std::endl;
			break;
		case 'w':
			if (currJoint == 0)
				currJoint = skeleton.joints[0];			
			currJoint->changeDOF(1, currJoint->dofX += 0.01);
			break;
		case 's':
			if (currJoint == 0)
				currJoint = skeleton.joints[0];
			currJoint->changeDOF(1, currJoint->dofX -= 0.01);
			break;
		case 'a':
			if (currJoint == 0)
				currJoint = skeleton.joints[0];
			currJoint->changeDOF(2, currJoint->dofY -= 0.01);
			break;
		case 'd':
			if (currJoint == 0)
				currJoint = skeleton.joints[0];
			currJoint->changeDOF(2, currJoint->dofY += 0.01);
			break;
		case 'z':
			if (currJoint == 0)
				currJoint = skeleton.joints[0];
			currJoint->changeDOF(3, currJoint->dofZ -= 0.01);
			break;
		case 'x':
			if (currJoint == 0)
				currJoint = skeleton.joints[0];
			currJoint->changeDOF(3, currJoint->dofZ += 0.01);
			break;
		/*case 'm':
			toMorph = !toMorph; 
			std::cout << toMorph << std::endl;
			if (toMorph)
			{				
				this->skin.loadMorph(morph.c_str());							
				this->skin.loadMorph(morph2.c_str());
			}
			else
			{
				this->skin.load(skinName.c_str());
			}
			break;*/
	}
}

////////////////////////////////////////////////////////////////////////////////

void Tester::MouseButton(int btn,int state,int x,int y) {
	if(btn==GLUT_LEFT_BUTTON) {
		LeftDown = (state==GLUT_DOWN);
	}
	else if(btn==GLUT_MIDDLE_BUTTON) {
		MiddleDown = (state==GLUT_DOWN);
	}
	else if(btn==GLUT_RIGHT_BUTTON) {
		RightDown = (state==GLUT_DOWN);
	}
}

////////////////////////////////////////////////////////////////////////////////

void Tester::MouseMotion(int nx,int ny) {
	int dx = nx - MouseX;
	int dy = -(ny - MouseY);

	MouseX = nx;
	MouseY = ny;

	// Move camera
	// NOTE: this should really be part of Camera::Update()
	if(LeftDown) {
		const float rate=1.0f;
		Cam.SetAzimuth(Cam.GetAzimuth()+dx*rate);
		Cam.SetIncline(Cam.GetIncline()-dy*rate);
	}
	if(RightDown) {
		const float rate=0.01f;
		Cam.SetDistance(Cam.GetDistance()*(1.0f-dx*rate));
	}
}

////////////////////////////////////////////////////////////////////////////////
