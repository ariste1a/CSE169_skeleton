#ifndef _VECTOR4_H_
#define _VECTOR4_H_

class Vector4 
{
	protected:
		double v1; // vector coordinate one
		double v2; // vector coordinate two
		double v3; // vector coordinate three
		double w; // homogenous coordinate

	public:
		Vector4(); // default constructor
		Vector4(double, double, double); // constructor with three parameters for vector coordinates 
		Vector4(double, double, double, double); // constructor with four parameters
		double magnitude();
		void setV1(double);
		void setV2(double);
		void setV3(double);
		void setW(double);
		double getV1();
		double getV2();
		double getV3();
		double getW();
		Vector4 add(Vector4);
		Vector4 operator+(Vector4);
		Vector4 subtract(Vector4);
		Vector4 operator-(Vector4);
		double operator[](int);

		void dehomogenize();
		Vector4 scale(double);
		void normalize();
		void print();
		void crossProduct(Vector4);

		double multiply(Vector4);
		double angle(Vector4&);
		friend class Bezier;
};

#endif