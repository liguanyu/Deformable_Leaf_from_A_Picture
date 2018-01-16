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

//储存特殊点
class Ratio_Point {
public:
	int index;//对应点在v_list中的索引
	int veinIndex; //-1 means egde of leaf, -2 means the mainvein
	float ratio_x; //存储计算用的ratio
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

	vector<Point> v_list; //顶点表
	vector<int> layer_index; //size = layerNum + 1 ，每层的开始索引
	vector<int> layer_mid_index;//每层的中点索引
	int layerNum;
	int setup_layer_mid_index();
	int index_to_layer(int index);

	vector<Triangle> v_index;//每个三角形对应的顶点索引

	GLint texGround;//纹理

	bool texture_on = 1;

	int layer_distance; //层与层间距

	int setup();

	vector<BezierLine> vein; //侧脉
	vector<vector<Ratio_Point>> point_with_ratio; //特殊点（主脉点，侧脉点，边缘点）
	int veinNum;
	int modify_mesh_vertex_to_vein(); //根据侧脉改造网格，让其与侧脉建立联系

	vector<int> veinStartIndex; //位于主脉上的侧脉的控制点

	BezierLine mainvein; //主脉
	int modify_mesh_vertex_to_mainvein(); //建立主脉

	vector<int> leftVeinEgde; //左叶边缘上的点
	vector<int> rightVeinEgde;//右叶边缘上的点

	int deform_with_mainvein(); //变形
	int deform_with_vein(); //变形

	int draw();
	int draw_edge();

	int cal_normal(); //计算每个顶点的法矢量，未实现光照所以没用上

	int cal_vein_from_2point(vector<vector<Point>> startAndEndofVein); //通过鼠标点击点建立侧脉

	int set(const vector<vector<float>> &vvv, float distance);

};


#endif // !LEAF_LGY_H
