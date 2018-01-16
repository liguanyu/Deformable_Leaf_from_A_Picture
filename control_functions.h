#pragma once
#ifndef CONTROL_FUNCTIONS_H
#define CONTROL_FUNCTIONS_H

#include <gl/glew.h>

#include <windows.h>
#include <gl/glut.h>
#include <math.h>
#include "gen_vertices.h"
#include <iostream>
#include <vector>
#include <algorithm>

#include "leaf_lgy.h"
#include "read_img.h"

void initial_lgy_project(); 
void rendererScene(void); //渲染
void setupRC(void); //背景
void rotateMode(int key, int x, int y); //键盘控制旋转
void choose_ctrlP_mode(unsigned char key, int x, int y); //鼠标选取最近控制点
void myMouse(int button, int state, int x, int y); //鼠标点击事件



#endif // !CONTROL_FUNCTIONS_H
