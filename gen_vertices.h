#ifndef GEN_VERTICES_H
#define GEN_VERTICES_H

#include <iostream>
#include <vector>
#include "point_lgy.h"

using namespace std;


#define get_height(x)  (10*x*x/10000)

//��Щ������������������������񶥵�İ���
vector<vector<float>> generate_vertices(vector<float> width, int layers);
int gen_vertices_and_index(vector<Point> &v_list, vector<Triangle> &v_index, vector<int> &layer_index, const vector<vector<float>> &vvv, float distance);



#endif // !GEN_VERTICES_H

