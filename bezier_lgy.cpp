#include "bezier_lgy.h"
#include <fstream>
using namespace std;

static float cal_coef_n_k(int n, int k) 
{
	int i;
	double sum = 1;
	for (i = 1; i <= n; i++)
		sum *= i;
	for (i = 1; i <= k; i++)
		sum /= i;
	for (i = 1; i <= n - k; i++)
		sum /= i;
	return sum;
}



BezierLine::BezierLine(vector<Point> ctrlP) {
	this->ctrlPointNum = ctrlP.size();
	this->ctrlPoints = ctrlP;
	this->calculate_coefficient();
}

int BezierLine::set(vector<Point> ctrlP) {
	this->ctrlPointNum = ctrlP.size();
	this->ctrlPoints = ctrlP;
	this->calculate_coefficient();
	return 1;
}

int BezierLine::get_ctrlP(Point &p, int index) {
	if (index < 0 || index >= (int)this->ctrlPoints.size())
		return 0;

	p = this->ctrlPoints[index];
	return 1;
}

int BezierLine::set_ctrlP(Point p, int index) {
	if (index < 0 || index >= (int)this->ctrlPoints.size())
		return 0;

	this->ctrlPoints[index] = p;
	return 1;
}

int BezierLine::calculate_coefficient() {
	int n = this->ctrlPointNum - 1;
	float c = 0;
	float t = 1 / BZlineLength;

	this->coefficient.clear();

	for (int i = 0; i < this->ctrlPointNum; i++) {
		c = cal_coef_n_k(n, i);
		this->coefficient.push_back(c);
	}
	return 1;
}

int BezierLine::refresh() {
	float x, y, z, t;
	int n = this->ctrlPointNum - 1;
	Point p;
	for (int i = 0; i < BZlineLength; i++) {
		x = y = z = 0;
		t = (float)i / (BZlineLength - 1);
		for (int j = 0; j < this->ctrlPointNum; j++) {
			x += this->coefficient[j] * this->ctrlPoints[j].x * pow(t, j) * pow(1 - t, n - j);
			y += this->coefficient[j] * this->ctrlPoints[j].y * pow(t, j) * pow(1 - t, n - j);
			z += this->coefficient[j] * this->ctrlPoints[j].z * pow(t, j) * pow(1 - t, n - j);
		}
		p.set(x, y, z);
		this->drawPoints[i] = p;
	}

	return 1;

}



int BezierLine::draw() {

	glLineWidth(2);

	glColor3f(243.0/255, 1, 152.0/255);
	glBegin(GL_LINE_STRIP);
	for (Point p : this->drawPoints)
	{
		glVertex3f(p.x, p.y, p.z+4);
	}
	glEnd();

	glBegin(GL_LINE_STRIP);
	for (Point p : this->drawPoints)
	{
		glVertex3f(p.x, p.y, p.z-4);
	}
	glEnd();

	glFlush();

	return 1;
}

int BezierLine::draw_ctrl_points() {
	glLineWidth(3);
	glBegin(GL_LINE_STRIP);
	glColor3f(0, 0, 1);
	for (Point p : ctrlPoints)
	{
		glVertex3f(p.x, p.y, p.z);
	}
	glEnd();
	glFlush();


	return 1;
}

Point BezierLine::get_point(float ratio)
{
	float x, y, z, t;
	t = ratio;


	int n = this->ctrlPointNum - 1;
	Point p;
	x = y = z = 0;
	for (int j = 0; j < this->ctrlPointNum; j++) {
		x += this->coefficient[j] * this->ctrlPoints[j].x * pow(t, j) * pow(1 - t, n - j);
		y += this->coefficient[j] * this->ctrlPoints[j].y * pow(t, j) * pow(1 - t, n - j);
		z += this->coefficient[j] * this->ctrlPoints[j].z * pow(t, j) * pow(1 - t, n - j);
	}
	p.set(x, y, z);
	return p;
}


int BezierLine::change_ctrlP(Point p, int index) {
	if (index >= this->ctrlPointNum)
		return 0;

	this->ctrlPoints[index] = p;
	this->refresh();

	return 1;
}

int BezierLine::change_ctrlP(int index, int mode, float offset) {
	if (index >= this->ctrlPointNum)
		return 0;

	if (mode == 1) {
		this->ctrlPoints[index].x += offset;
	}
	else if (mode == 2) {
		this->ctrlPoints[index].y += offset;
	}
	else {
		this->ctrlPoints[index].z += offset;
	}
	this->refresh();

	return 1;
}