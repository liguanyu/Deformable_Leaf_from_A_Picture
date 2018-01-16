#include <gl/glew.h>

#include <windows.h>
#include <gl/glut.h>
#include <math.h>
#include "gen_vertices.h"
#include <iostream>
#include <vector>
#include <algorithm>


#include "leaf_lgy.h"
#include "control_functions.h"

using namespace std;

int main(int argc, char* argv[])
{
	//设置显示模式
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	//设置窗口大小
	glutInitWindowSize(600, 800);

	//设置窗口在屏幕上的位置
	glutInitWindowPosition(700, 200);

	//创建窗口标题
	glutCreateWindow("Leaf Model");

	cout << "init glew" << endl;
	glewExperimental = GL_TRUE;
	glewInit();

	initial_lgy_project();

	//注册显示窗口时回调渲染函数

	glutMouseFunc(myMouse);



	//注册点击上下左右方向按钮时回调rotateMode函数
	glutSpecialFunc(rotateMode);
	glutKeyboardFunc(choose_ctrlP_mode);


	setupRC();

	cout << "单击鼠标来构建叶脉" << endl
		<< "第一次单击表明叶脉起点" << endl
		<< "第二次单击表明叶脉终点" << endl
		<< "重复此过程" << endl
		<< "请保证叶脉终点高于起点（y方向上）" << endl
		<< "请保证叶脉不会交叉" << endl
		<< "单击右键确认" << endl;

	//消息循环（处理操作系统等的消息，例如键盘、鼠标事件等）
	glutMainLoop();
	return 0;
}

