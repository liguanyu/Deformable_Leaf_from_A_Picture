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
	vector<Point> ctrlPoints;//控制点

	Point drawPoints[BZlineLength];//绘制点
	vector<float> coefficient;//计算中间系数


	BezierLine(vector<Point> ctrlP);
	BezierLine(){}

	int set(vector<Point> ctrlP);

	int get_ctrlP(Point &p, int index);

	int set_ctrlP(Point p, int index);

	int calculate_coefficient();

	int refresh();
	

	int draw();

	int draw_ctrl_points();

	Point get_point(float ratio); //计算处于bezier曲线上处于ratio处的值

	int change_ctrlP(Point p, int index); //改变控制点
	int change_ctrlP(int index, int mode, float offset);

};


#endif // !BEZIER_LGY_H


