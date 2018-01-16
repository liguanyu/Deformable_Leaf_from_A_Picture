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
void rendererScene(void); //��Ⱦ
void setupRC(void); //����
void rotateMode(int key, int x, int y); //���̿�����ת
void choose_ctrlP_mode(unsigned char key, int x, int y); //���ѡȡ������Ƶ�
void myMouse(int button, int state, int x, int y); //������¼�



#endif // !CONTROL_FUNCTIONS_H
