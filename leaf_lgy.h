#ifndef LEAF_LGY_H
#define LEAF_LGY_H

#include <gl/glew.h>
#include <windows.h>
#include <gl/glut.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>


#include "bezier_lgy.h"
#include "point_lgy.h"
#include "gen_vertices.h"
#include "load_texture.h"

#pragma comment(lib, "glew32.lib")
using namespace std;

//���������
class Ratio_Point {
public:
	int index;//��Ӧ����v_list�е�����
	int veinIndex; //-1 means egde of leaf, -2 means the mainvein
	float ratio_x; //�洢�����õ�ratio
	float ratio_y;
	float ratio_z;

	Ratio_Point(int ind, int vi, float rt)
		: index(ind),veinIndex(vi),ratio_x(rt), ratio_y(rt), ratio_z(rt){}
	Ratio_Point(int ind, int vi, float rt_x, float rt_y, float rt_z)
		: index(ind), veinIndex(vi), ratio_x(rt_x), ratio_y(rt_y), ratio_z(rt_z) {}
};


class Leaf {
public:
	Leaf(const vector<vector<float>> &vvv, float distance, vector<BezierLine> v);
	Leaf() {}
	int set(const vector<vector<float>> &vvv, float distance, vector<BezierLine> v);

	vector<Point> v_list; //�����
	vector<int> layer_index; //size = layerNum + 1 ��ÿ��Ŀ�ʼ����
	vector<int> layer_mid_index;//ÿ����е�����
	int layerNum;
	int setup_layer_mid_index();
	int index_to_layer(int index);

	vector<Triangle> v_index;//ÿ�������ζ�Ӧ�Ķ�������

	GLint texGround;//����

	bool texture_on = 1;

	int layer_distance; //�������

	int setup();

	vector<BezierLine> vein; //����
	vector<vector<Ratio_Point>> point_with_ratio; //����㣨�����㣬�����㣬��Ե�㣩
	int veinNum;
	int modify_mesh_vertex_to_vein(); //���ݲ��������������������������ϵ

	vector<int> veinStartIndex; //λ�������ϵĲ����Ŀ��Ƶ�

	BezierLine mainvein; //����
	int modify_mesh_vertex_to_mainvein(); //��������

	vector<int> leftVeinEgde; //��Ҷ��Ե�ϵĵ�
	vector<int> rightVeinEgde;//��Ҷ��Ե�ϵĵ�

	int deform_with_mainvein(); //����
	int deform_with_vein(); //����

	int draw();
	int draw_edge();

	int cal_normal(); //����ÿ������ķ�ʸ����δʵ�ֹ�������û����

	int cal_vein_from_2point(vector<vector<Point>> startAndEndofVein); //ͨ��������㽨������

	int set(const vector<vector<float>> &vvv, float distance);

};


#endif // !LEAF_LGY_H
