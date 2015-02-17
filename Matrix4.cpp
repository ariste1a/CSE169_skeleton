#include <math.h>
#include <stdio.h>
#include <iostream>
#include "Matrix4.h"

Matrix4::Matrix4()
{
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			m[i][j] = 0;
		}
	}
}
Matrix4::Matrix4(
	double m00, double m01, double m02, double m03,
	double m10, double m11, double m12, double m13,
	double m20, double m21, double m22, double m23,
	double m30, double m31, double m32, double m33)
{
	m[0][0] = m00;
	m[0][1] = m01;
	m[0][2] = m02;
	m[0][3] = m03;
	m[1][0] = m10;
	m[1][1] = m11;
	m[1][2] = m12;
	m[1][3] = m13;
	m[2][0] = m20;
	m[2][1] = m21;
	m[2][2] = m22;
	m[2][3] = m23;
	m[3][0] = m30;
	m[3][1] = m31;
	m[3][2] = m32;
	m[3][3] = m33;
}

double* Matrix4::getPointer()
{
	return &m[0][0];
}

void Matrix4::identity()
{
	double ident[4][4] = { { 1, 0, 0, 0 }, { 0, 1, 0, 0 }, { 0, 0, 1, 0 }, { 0, 0, 0, 1 } };
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			m[i][j] = ident[i][j];
		}
	}
}

double Matrix4::getElement(int y, int x)
{
	return m[y][x];
}

// angle in radians
void Matrix4::rotateY(double angle)
{
	m[0][0] = cos(angle);
	m[0][1] = 0;
	m[0][2] = sin(angle);
	m[0][3] = 0;
	m[1][0] = 0;
	m[1][1] = 1;
	m[1][2] = 0;
	m[1][3] = 0;
	m[2][0] = -sin(angle);
	m[2][1] = 0;
	m[2][2] = cos(angle);
	m[2][3] = 0;
	m[3][0] = 0;
	m[3][1] = 0;
	m[3][2] = 0;
	m[3][3] = 1;
}

void Matrix4::rotateX(double angle)
{
	m[0][0] = 1;
	m[0][1] = 0;
	m[0][2] = 0;
	m[0][3] = 0;
	m[1][0] = 0;
	m[1][1] = cos(angle);
	m[1][2] = sin(angle);
	m[1][3] = 0;
	m[2][0] = 0;
	m[2][1] = -sin(angle);
	m[2][2] = cos(angle);
	m[2][3] = 0;
	m[3][0] = 0;
	m[3][1] = 0;
	m[3][2] = 0;
	m[3][3] = 1;
}

Matrix4 Matrix4::rotate(double angle, double x, double y, double z)
{
	double rot[4][4];

	rot[0][0] = 1 + (1 - cos(angle))*(x*x - 1);
	rot[0][1] = -(z *sin(angle)) + (1 - cos(angle))*x*y;
	rot[0][2] = (y *sin(angle)) + (1 - cos(angle))*x*z;
	rot[0][3] = 0;
	rot[1][0] = (z *sin(angle)) + (1 - cos(angle))*y*x;
	rot[1][1] = 1 + (1 - cos(angle))*(y*y - 1);
	rot[1][2] = -(x *sin(angle)) + (1 - cos(angle))*y*z;
	rot[1][3] = 0;
	rot[2][0] = -(y *sin(angle)) + (1 - cos(angle))*z*x;
	rot[2][1] = (x *sin(angle)) + (1 - cos(angle))*z*y;;
	rot[2][2] = 1 + (1 - cos(angle))*(z*z - 1);
	rot[2][3] = 0;
	rot[3][0] = 0;
	rot[3][1] = 0;
	rot[3][2] = 0;
	rot[3][3] = 1;
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			m[i][j] = rot[i][j];
		}
	}
	return(Matrix4(rot[0][0], rot[0][1], rot[0][2], rot[0][3],
		rot[1][0], rot[1][1], rot[1][2], rot[1][3],
		rot[2][0], rot[2][1], rot[2][2], rot[2][3],
		rot[3][0], rot[3][1], rot[3][2], rot[3][3]));

}

// Initialize the projection matrix (frustrum)
void Matrix4::setFrustum(double left, double right, double top, double bottom, double near, double far)
{
	m[0][0] = (2 * near) / (right - left);
	m[0][1] = 0;
	m[0][2] = (right + left) / (right - left);
	m[0][3] = 0;

	m[1][0] = 0;
	m[1][1] = (2 * near) / (top - bottom);
	m[1][2] = (top + bottom) / (top - bottom);
	m[1][3] = 0;

	m[2][0] = 0;
	m[2][1] = 0;
	m[2][2] = -(near + far) / (near - far);
	m[2][3] = -(2 * near * far) / (near - far);

	m[3][0] = 0;
	m[3][1] = 0;
	m[3][2] = -1;
	m[3][3] = 0;

}

// Initialize the viewport transformation matrix
void Matrix4::setViewPort(double width, double height)
{
	m[0][0] = width / 2;
	m[0][1] = 0;
	m[0][2] = 0;
	m[0][3] = width / 2;

	m[1][0] = 0;
	m[1][1] = height / 2;
	m[1][2] = 0;
	m[1][3] = height / 2;

	m[2][0] = 0;
	m[2][1] = 0;
	m[2][2] = 0.5;
	m[2][3] = 0.5;

	m[3][0] = 0;
	m[3][1] = 0;
	m[3][2] = 0;
	m[3][3] = 1;

}

Matrix4 Matrix4::multiply(Matrix4 matrix)
{
	double temp1[4][4];
	double temp2[4][4];

	for (int i = 0; i < 4; ++i){
		for (int j = 0; j < 4; ++j)
		{
			temp2[i][j] = matrix.getElement(i, j);
		}
	}

	temp1[0][0] = m[0][0] * temp2[0][0] + m[0][1] * temp2[1][0] + m[0][2] * temp2[2][0] + m[0][3] * temp2[3][0];
	temp1[0][1] = m[0][0] * temp2[0][1] + m[0][1] * temp2[1][1] + m[0][2] * temp2[2][1] + m[0][3] * temp2[3][1];
	temp1[0][2] = m[0][0] * temp2[0][2] + m[0][1] * temp2[1][2] + m[0][2] * temp2[2][2] + m[0][3] * temp2[3][2];
	temp1[0][3] = m[0][0] * temp2[0][3] + m[0][1] * temp2[1][3] + m[0][2] * temp2[2][3] + m[0][3] * temp2[3][3];
	temp1[1][0] = m[1][0] * temp2[0][0] + m[1][1] * temp2[1][0] + m[1][2] * temp2[2][0] + m[1][3] * temp2[3][0];
	temp1[1][1] = m[1][0] * temp2[0][1] + m[1][1] * temp2[1][1] + m[1][2] * temp2[2][1] + m[1][3] * temp2[3][1];
	temp1[1][2] = m[1][0] * temp2[0][2] + m[1][1] * temp2[1][2] + m[1][2] * temp2[2][2] + m[1][3] * temp2[3][2];
	temp1[1][3] = m[1][0] * temp2[0][3] + m[1][1] * temp2[1][3] + m[1][2] * temp2[2][3] + m[1][3] * temp2[3][3];
	temp1[2][0] = m[2][0] * temp2[0][0] + m[2][1] * temp2[1][0] + m[2][2] * temp2[2][0] + m[2][3] * temp2[3][0];
	temp1[2][1] = m[2][0] * temp2[0][1] + m[2][1] * temp2[1][1] + m[2][2] * temp2[2][1] + m[2][3] * temp2[3][1];
	temp1[2][2] = m[2][0] * temp2[0][2] + m[2][1] * temp2[1][2] + m[2][2] * temp2[2][2] + m[2][3] * temp2[3][2];
	temp1[2][3] = m[2][0] * temp2[0][3] + m[2][1] * temp2[1][3] + m[2][2] * temp2[2][3] + m[2][3] * temp2[3][3];
	temp1[3][0] = m[3][0] * temp2[0][0] + m[3][1] * temp2[1][0] + m[3][2] * temp2[2][0] + m[3][3] * temp2[3][0];
	temp1[3][1] = m[3][0] * temp2[0][1] + m[3][1] * temp2[1][1] + m[3][2] * temp2[2][1] + m[3][3] * temp2[3][1];
	temp1[3][2] = m[3][0] * temp2[0][2] + m[3][1] * temp2[1][2] + m[3][2] * temp2[2][2] + m[3][3] * temp2[3][2];
	temp1[3][3] = m[3][0] * temp2[0][3] + m[3][1] * temp2[1][3] + m[3][2] * temp2[2][3] + m[3][3] * temp2[3][3];

	return Matrix4(temp1[0][0], temp1[0][1], temp1[0][2], temp1[0][3],
		temp1[1][0], temp1[1][1], temp1[1][2], temp1[1][3],
		temp1[2][0], temp1[2][1], temp1[2][2], temp1[2][3],
		temp1[3][0], temp1[3][1], temp1[3][2], temp1[3][3]);
}

Vector4 Matrix4::multiply(Vector4 vector)
{
	double d0, d1, d2, d3;
	d0 = m[0][0] * vector.getV1() + m[0][1] * vector.getV2() + m[0][2] * vector.getV3() + m[0][3] * vector.getW();
	d1 = m[1][0] * vector.getV1() + m[1][1] * vector.getV2() + m[1][2] * vector.getV3() + m[1][3] * vector.getW();
	d2 = m[2][0] * vector.getV1() + m[2][1] * vector.getV2() + m[2][2] * vector.getV3() + m[2][3] * vector.getW();
	d3 = m[3][0] * vector.getV1() + m[3][1] * vector.getV2() + m[3][2] * vector.getV3() + m[3][3] * vector.getW();
	//std::cout << d0 << d1 << d2 << d3 << std::endl;
	return (Vector4(d0, d1, d2, d3));
}

void Matrix4::print()
{
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			std::cout << m[i][j] << " ";
		}
		std::cout << std::endl;
	}
}

void Matrix4::translate(double x, double y, double z)
{
	m[0][0] = 1;
	m[0][1] = 0;
	m[0][2] = 0;
	m[0][3] = x;

	m[1][0] = 0;
	m[1][1] = 1;
	m[1][2] = 0;
	m[1][3] = y;

	m[2][0] = 0;
	m[2][1] = 0;
	m[2][2] = 1;
	m[2][3] = z;

	m[3][0] = 0;
	m[3][1] = 0;
	m[3][2] = 0;
	m[3][3] = 1;
}

void Matrix4::transpose()
{
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			if (i < j){
				double temp = m[i][j];
				m[i][j] = m[j][i];
				m[j][i] = temp;
			}
		}
	}
}

void Matrix4::scale(double s)
{
	m[0][0] = s;
	m[0][1] = 0;
	m[0][2] = 0;
	m[0][3] = 0;

	m[1][0] = 0;
	m[1][1] = s;
	m[1][2] = 0;
	m[1][3] = 0;

	m[2][0] = 0;
	m[2][1] = 0;
	m[2][2] = s;
	m[2][3] = 0;

	m[3][0] = 0;
	m[3][1] = 0;
	m[3][2] = 0;
	m[3][3] = 1;
}

void Matrix4::copyRot(Matrix4& r)
{
	double temp[4][4];

	for (int i = 0; i < 4; ++i){
		for (int j = 0; j < 4; ++j)
		{
			temp[i][j] = r.getElement(i, j);
		}
	}
	m[0][0] = temp[0][0];
	m[0][1] = temp[0][0];
	m[0][2] = temp[0][0];

	m[1][0] = temp[0][0];
	m[1][1] = temp[0][0];
	m[1][2] = temp[0][0];

	m[2][0] = temp[0][0];
	m[2][1] = temp[0][0];
	m[2][2] = temp[0][0];

}

/** Rotates the matrix according to a fictitious trackball, placed in
the middle of the given window.
The trackball is approximated by a Gaussian curve.
The trackball coordinate system is: x=right, y=up, z=to viewer<BR>
The origin of the mouse coordinates zero (0,0) is considered to be top left.
@param width, height  window size in pixels
@param fromX, fromY   mouse starting position in pixels
@param toX, toY       mouse end position in pixels
*/
Matrix4 Matrix4::trackballRotation(int width, int height, int fromX, int fromY, int toX, int toY)
{
	const float TRACKBALL_SIZE = 1.3f;              // virtual trackball size (empirical value)
	Matrix4 mInv;                                   // inverse of ObjectView matrix
	Vector4 v1, v2;                                 // mouse drag positions in normalized 3D space
	float smallSize;                                // smaller window size between width and height
	float halfWidth, halfHeight;                    // half window sizes
	float angle;                                    // rotational angle
	float d;                                        // distance

	// Compute mouse coordinates in window and normalized to -1..1
	// ((0,0)=window center, (-1,-1) = bottom left, (1,1) = top right)
	halfWidth = (float)width / 2.0f;
	halfHeight = (float)height / 2.0f;
	smallSize = (halfWidth < halfHeight) ? halfWidth : halfHeight;
	v1.setV1(((float)fromX - halfWidth) / smallSize);
	v1.setV2(((float)(height - fromY) - halfHeight) / smallSize);
	v2.setV1(((float)toX - halfWidth) / smallSize);
	v2.setV2(((float)(height - toY) - halfHeight) / smallSize);

	// Compute z-coordinates on Gaussian trackball:
	d = sqrtf(v1[0] * v1[0] + v1[1] * v1[1]);
	v1.setV3(expf(-TRACKBALL_SIZE * d * d));
	d = sqrtf(v2[0] * v2[0] + v2[1] * v2[1]);
	v2.setV3(expf(-TRACKBALL_SIZE * d * d));

	// Compute rotational angle:
	angle = v1.angle(v2);                          // angle = angle between v1 and v2
	
	// Compute rotational axis:
	v2.crossProduct(v1);                                  // v2 = v2 x v1 (cross product)

	// Convert axis coordinates (v2) from WCS to OCS:
	mInv.identity();
	//mInv.copyRot(*this);                             // copy rotational part of mv to mInv
	mInv.transpose();                             // invert orthogonal matrix mInv
	
	v2 = mInv.multiply(v2);                             // v2 = v2 x mInv (matrix multiplication)
	v2.normalize();                                 // normalize v2 before rotation

	// Perform acutal model view matrix modification:
	return rotate(-angle, v2[0], v2[1], v2[2]);      // rotate model view matrix
}
