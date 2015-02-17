#include <math.h>
#include <iostream>
#include "Vector4.h"

// default constructor
Vector4::Vector4() 
{
	v1 = 0;
	v2 = 0;
	v3 = 0;
	w = 0;
}

Vector4::Vector4(double v01, double v02, double v03)
{
	v1 = v01;
	v2 = v02;
	v3 = v03;
	w = 0;
}

Vector4::Vector4(double v01, double v02, double v03, double w01)
{
	v1 = v01;
	v2 = v02;
	v3 = v03;
	w = w01;
}

void Vector4::setV1(double v01)
{
	v1 = v01;
}

void Vector4::setV2(double v02)
{
	v2 = v02;
}

void Vector4::setV3(double v03)
{
	v3 = v03;
}

void Vector4::setW(double w01)
{
	w = w01;
}

double Vector4::getV1()
{
	return v1;
}


double Vector4::getV2()
{
	return v2;
}

double Vector4::getV3()
{
	return v3;
}


double Vector4::getW()
{
	return w;
}

Vector4 Vector4::add(Vector4 v)
{
	this->setV1(this->v1 + v.v1);
	this->setV2(this->v2 + v.v2);
	this->setV3(this->v3 + v.v3);
	this->setW(this->w + v.w);
	return *this;
}

Vector4 Vector4::operator+(Vector4 v)
{
	return this->add(v);
}

Vector4 Vector4::subtract(Vector4 v)
{
	v1 -= v.v1;
	v2 -= v.v2;
	v3 -= v.v3;
	w -= v.w;
	return *this;
}

Vector4 Vector4::operator-(Vector4 v)
{
	return subtract(v);
}

double Vector4::operator[](int index)
{
	switch (index)
	{
	case 0:
		return v1;
	case 1:
		return v2;
	case 2:
		return v3;
	case 3:
		return w;
	default:
		std::cout << "Index Out of Bounds" << std::endl;
		break;
	}
}

double Vector4::magnitude()
{
	return(sqrt(pow(v1, 2) + pow(v2, 2) + pow(v3, 2) + pow(w, 2)));
}

void Vector4::dehomogenize()
{
	this->setV1((this->v1) / (this->w));
	this->setV2((this->v2) / (this->w));
	this->setV3((this->v3) / (this->w));
	this->setW((this->w) / (this->w));
}

Vector4 Vector4::scale(double scale)
{
	v1 *= scale;
	v2 *= scale;
	v3 *= scale;
	w *= scale;
	return *this;
}

void Vector4::crossProduct(Vector4 v)
{
	double v01 = v2* v.getV3() - v3 * v.getV2();
	double v02 = v1 * v.getV3() - v3 * v.getV1();
	double v03 = v1 * v.getV2() - v2 * v.getV1();
	v1 = v01;
	v2 = 0-v02;
	v3 = v03;
}

void Vector4::normalize()
{
	double mag = magnitude();
	v1 /= mag;
	v2 /= mag;
	v3 /= mag;
	w /= mag;
}

void Vector4::print()
{
	std::cout << "x: " << v1 << std::endl;
	std::cout << "y: " << v2 << std::endl;
	std::cout << "z: " << v3 << std::endl;
	std::cout << "w: " << w << std::endl;
}

// for Matrix4
double Vector4::multiply(Vector4 v)
{
	return ((this->v1 * v.v1) + (this->v2 * v.v2) + (this->v3 * v.v3) + (this->w * v.w));
}

double Vector4::angle(Vector4& a)
{
	double temp = this->multiply(a);
	temp /= this->magnitude()*a.magnitude();
	return acos(temp);
}
