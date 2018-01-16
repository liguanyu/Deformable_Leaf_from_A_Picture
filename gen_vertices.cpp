#include "gen_vertices.h"



vector<vector<float>>
generate_vertices(vector<float> width, int layers) {
	if (width[0] != 0)
		width.insert(width.begin(), 1, 0);

	int length = width.size();
	vector<float> newWidth(layers);
	vector<vector<float>> result;

	int space = layers / (int)(width.size()-1);

	int firstLy = layers - space*(int)(width.size()-1) + space;
	for (int i = 0; i < firstLy; i++) {
		newWidth[i] = width[0] + i * (width[1] - width[0]) / firstLy;
	}

	for (int i = 1; i < (int)width.size() - 1; i++) {
		float between = width[i + 1] - width[i];
		for (int j = 0; j < space; j++) {
			newWidth[firstLy + (i - 1)*space + j] = width[i] + j*between / space;
		}
	}
	newWidth[layers - 1] = *(width.end()-1);
	

	int index = 1;

	result.push_back(vector<float>(1, 0));
	int num = 1;
	int w = 1;


	for (; index < layers; index++) {
		if (newWidth[index] > newWidth[index - 1] || w < 20)
			w++;
		else if (newWidth[index] < newWidth[index - 1])
			w--;

		vector<float> temp(w, 0);
		float widthHere = newWidth[index];
		for (int i = 0; i < w; i++) {
			temp[i] = -widthHere / 2 + i*widthHere / (w - 1);
		}

		result.push_back(temp);
	}

	return result;

}

int gen_vertices_and_index(vector<Point> &v_list, vector<Triangle> &v_index, vector<int> &layer_index, const vector<vector<float>> &vvv, float distance)
{

	float y = 0;
	int layer_start_index = 0;

	v_list.clear();

	v_index.clear();

	layer_index.clear();

	for (auto &line : vvv) {
		layer_index.push_back(layer_start_index);
		for (auto &item : line) {
			Point p(item, y, 0);
			v_list.push_back(p);
		}
		layer_start_index += line.size();
		y -= distance;
	}
	layer_index.push_back(layer_start_index);

	for (int i = 0; i < (int)vvv.size() - 1; i++) {
		int longLine, shortLine = 0;
		int indexLong = 0;
		int indexShort = 0;
		if (vvv[i].size() > vvv[i + 1].size()) {
			longLine = i;
			shortLine = i + 1;
		}
		else {
			longLine = i + 1;
			shortLine = i;
		}

		for (int j = 0; j < longLine; j++) {
			indexLong += vvv[j].size();
		}

		for (int j = 0; j < shortLine; j++) {
			indexShort += vvv[j].size();
		}


		int length = vvv[shortLine].size();
		for (int j = 0; j < length - 1; j++) {

			Triangle t1(indexShort + j, indexLong + j, indexLong + j + 1);
			Triangle t2(indexShort + j, indexLong + j + 1, indexShort + j + 1);

			v_index.push_back(t1);
			v_index.push_back(t2);

		}

		if (vvv[longLine].size() > vvv[shortLine].size()) {
			Triangle t1(indexShort + length - 1, indexLong + length - 1, indexLong + length);
			v_index.push_back(t1);

		}
	}

	return 1;
}