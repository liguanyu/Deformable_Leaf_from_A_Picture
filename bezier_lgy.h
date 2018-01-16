#ifndef BEZIER_LGY_H
#define BEZIER_LGY_H


#include <gl/glew.h>
#include <windows.h>
#include <gl/glut.h>
#include <iostream>
#include <vector>
#include <cmath>

#include "point_lgy.h"

using namespace std;

#define BZlineLength 51



class BezierLine {
public:
	int ctrlPointNum;
	vector<Point> ctrlPoints;//���Ƶ�

	Point drawPoints[BZlineLength];//���Ƶ�
	vector<float> coefficient;//�����м�ϵ��


	BezierLine(vector<Point> ctrlP);
	BezierLine(){}

	int set(vector<Point> ctrlP);

	int get_ctrlP(Point &p, int index);

	int set_ctrlP(Point p, int index);

	int calculate_coefficient();

	int refresh();
	

	int draw();

	int draw_ctrl_points();

	Point get_point(float ratio); //���㴦��bezier�����ϴ���ratio����ֵ

	int change_ctrlP(Point p, int index); //�ı���Ƶ�
	int change_ctrlP(int index, int mode, float offset);

};


#endif // !BEZIER_LGY_H


