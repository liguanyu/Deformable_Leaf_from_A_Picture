#pragma once
#ifndef POINT_LGY_H
#define POINT_LGY_H

#include <gl/glew.h>
#include <windows.h>
#include <gl/glut.h>
#include <iostream>
#include <cmath>


class Point
{
public:
	GLfloat x;
	GLfloat y;
	GLfloat z;

	GLfloat nx;
	GLfloat ny;
	GLfloat nz;

	Point() {};
	Point(float a, float b, float c) {
		x = a;
		y = b;
		z = c;
	}
	void set(float a, float b, float c) {
		x = a;
		y = b;
		z = c;
	}

	void set_normal(float a, float b, float c) {
		nx = x - a;
		ny = y - b;
		nz = z - c;

		float sum = sqrt(nx*nx + ny*ny + nz*nz);
		nx /= sum;
		ny /= sum;
		nz /= sum;
	}
};

class Triangle {
public:
	int v1;
	int v2;
	int v3;
	Triangle(){}
	Triangle(int a, int b, int c) {
		v1 = a;
		v2 = b;
		v3 = c;
	}
};

#endif // !POINT_LGY_H
