#ifndef _MATRIX4_H_
#define _MATRIX4_H_
#include "Vector4.h"

class Matrix4
{
protected:

	double m[4][4];   // matrix elements

public:
	Matrix4();        // constructor
	Matrix4(double, double, double, double, double, double, double, double, double, double, double, double, double, double, double, double);
	double* getPointer();  // return pointer to matrix elements
	double getElement(int, int);
	void identity();  // create identity matrix
	void rotateY(double); // rotation about y axis
	void rotateX(double);
	Matrix4 rotate(double, double, double, double);

	void setFrustum(double, double, double, double, double, double);
	void setViewPort(double, double);
	void translate(double, double, double);
	void scale(double);
	void transpose();
	Vector4 multiply(Vector4);
	Matrix4 multiply(Matrix4);

	void copyRot(Matrix4&);

	void print();
	Matrix4 trackballRotation(int, int, int, int, int, int);

};

#endif