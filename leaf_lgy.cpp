#include "leaf_lgy.h"
using namespace std;



// less than
static bool comp(const Ratio_Point &a, const Ratio_Point &b)
{
	return a.index < b.index;
}

int Leaf::index_to_layer(int index)
{
	int result = 0;
	for (; result < layerNum; result++) {
		if (index < layer_index[result])
			break;
	}
	result--;
	return result;
}

int Leaf::setup_layer_mid_index()
{
	this->layer_mid_index.clear();

	int layerBegin, layerEnd;
	for (int i = 0; i < this->layerNum; i++) {
		layerBegin = this->layer_index[i];
		layerEnd = this->layer_index[i + 1] - 1;
		
		this->layer_mid_index.push_back((layerBegin + layerEnd)/2);

	}

	return 1;
}

int Leaf::modify_mesh_vertex_to_mainvein() //call by modify_mesh_vertex_to_vein()
{
	int mainVeinStartIndex = this->layer_mid_index[0];
	int mainVeinEndIndex = this->layer_mid_index[this->layerNum - 1];

	int mainVeinMidIndex1 = this->layer_mid_index[(this->layerNum - 1) / 3];
	int mainVeinMidIndex2 = this->layer_mid_index[2 * (this->layerNum - 1) / 3];

	vector<Point> ctrlP{ this->v_list[mainVeinStartIndex]
		, this->v_list[mainVeinMidIndex1]
		, this->v_list[mainVeinMidIndex2]
		, this->v_list[mainVeinEndIndex] };

	this->mainvein.set(ctrlP);

	vector<vector<Ratio_Point>> blankVector(layerNum);
	this->point_with_ratio = blankVector;

	for (int layer = 0; layer < this->layerNum; layer++) {
		int index = this->layer_mid_index[layer];
		float ratio = (float)(layer - 0) / (this->layerNum - 1);
		Ratio_Point rp(index, -2, ratio);
		this->point_with_ratio[layer].push_back(rp);
	}

	return 1;
}

int Leaf::modify_mesh_vertex_to_vein() // just call once
{
	this->modify_mesh_vertex_to_mainvein();

	float veinStartX, veinStartY, veinEndX, veinEndY;
	for (int v = 0; v < this->veinNum; v++) {
		veinStartX = this->vein[v].ctrlPoints[0].x;
		veinStartY = this->vein[v].ctrlPoints[0].y;
		veinEndX = this->vein[v].ctrlPoints.back().x;
		veinEndY = this->vein[v].ctrlPoints.back().y;

		//if (abs(veinStartX) > int(veinEndX)) {
		//	int tmp = veinEndX;
		//	veinEndX = veinStartX;
		//	veinStartX = tmp;

		//	tmp = veinEndY;
		//	veinEndY = veinStartY;
		//	veinStartY = tmp;
		//}



		for (int layer = 0; layer < this->layerNum; layer++) {
			float layer_y = this->v_list[this->layer_index[layer]].y;
			if ((layer_y >= veinStartY && layer_y <= veinEndY)
				|| (layer_y <= veinStartY && layer_y >= veinEndY)) {

				float ratio = (layer_y - veinStartY) / (veinEndY - veinStartY);
				float layer_x = veinStartX + ratio*(veinEndX - veinStartX);

				int index;
				for (index = this->layer_index[layer]; index < this->layer_index[layer + 1] - 1; index++) {
					if (this->v_list[index].x<=layer_x && this->v_list[index+1].x>=layer_x)
						break;
				}

				if (layer_x > 0)
					index += 1;

				Ratio_Point rp(index, v, ratio);
				point_with_ratio[layer].push_back(rp);
				this->v_list[index].set(layer_x, layer_y, this->vein[v].ctrlPoints[0].z);
			}
		}
	}

	// setup left egde
	this->leftVeinEgde.push_back(this->layer_mid_index[0]);
	for (int v = 0; v < this->veinNum; v++) {
		if (this->vein[v].ctrlPoints.back().x > 0)
			break;

		int layer = abs((int)(this->vein[v].ctrlPoints.back().y) / this->layer_distance);
		this->leftVeinEgde.push_back(this->layer_index[layer]);
	}
	this->leftVeinEgde.push_back(layer_mid_index.back());

	if (this->leftVeinEgde.size() > 2) {
		for (int i = 0; i < (int)this->leftVeinEgde.size() - 1; i++) {
			float StartX, StartY, EndX, EndY;
			int startIndex = this->leftVeinEgde[i];
			int endIndex = this->leftVeinEgde[i + 1];
			int startLayer = this->index_to_layer(startIndex);
			int endLayer = this->index_to_layer(endIndex);

			StartX = this->v_list[startIndex].x;
			StartY = this->v_list[startIndex].y;
			EndX = this->v_list[endIndex].x;
			EndY = this->v_list[endIndex].y;


			for (int layer = startLayer + 1; layer < endLayer; layer++) {
				int index = this->layer_index[layer];
				float ratio_x = (this->v_list[index].x - StartX) / (EndX - StartX);
				float ratio_y = (this->v_list[index].y - StartY) / (EndY - StartY);

				Ratio_Point rp(index, -1, ratio_x, ratio_y, ratio_y);
				this->point_with_ratio[layer].push_back(rp);
			}
		}
	}

	// setup right egde
	this->rightVeinEgde.push_back(this->layer_mid_index[0]);
	for (int v = 0; v < this->veinNum; v++) {
		if (this->vein[v].ctrlPoints.back().x < 0)
			continue;

		int layer = abs((int)(this->vein[v].ctrlPoints.back().y) / this->layer_distance);
		this->rightVeinEgde.push_back(this->layer_index[layer + 1] - 1);
	}
	this->rightVeinEgde.push_back(layer_mid_index.back());

	if (this->rightVeinEgde.size() > 2) {
		for (int i = 0; i < (int)this->rightVeinEgde.size() - 1; i++) {
			float StartX, StartY, EndX, EndY;
			int startIndex = this->rightVeinEgde[i];
			int endIndex = this->rightVeinEgde[i + 1];
			int startLayer = this->index_to_layer(startIndex);
			int endLayer = this->index_to_layer(endIndex);

			StartX = this->v_list[startIndex].x;
			StartY = this->v_list[startIndex].y;
			EndX = this->v_list[endIndex].x;
			EndY = this->v_list[endIndex].y;
			
			
			for (int layer = startLayer + 1; layer < endLayer; layer++) {
				int index = this->layer_index[layer + 1] - 1;
				float ratio_x = (this->v_list[index].x - StartX) / (EndX - StartX);
				float ratio_y = (this->v_list[index].y - StartY) / (EndY - StartY);

				Ratio_Point rp(index, -1, ratio_x, ratio_y, ratio_y);
				this->point_with_ratio[layer].push_back(rp);
			}
		}
	}

	// sort point_with_ratio
	for (int i = 0; i < this->layerNum; i++) {
		sort(this->point_with_ratio[i].begin(), this->point_with_ratio[i].end(), comp);
	}
	
	return 1;
}

int Leaf::deform_with_mainvein() {
	for (int layer = 0; layer < this->layerNum; layer++) {
		int index = this->layer_mid_index[layer];
		float ratio = (float)(layer - 0) / (this->layerNum - 1);
		Point p = this->mainvein.get_point(ratio);


		this->v_list[this->layer_mid_index[layer]].set(p.x, p.y, p.z);
	}

	for (int v = 0; v < this->veinNum; v++) {
		this->vein[v].set_ctrlP(this->v_list[this->veinStartIndex[v]], 0);
	}

	return 1;
}

int Leaf::deform_with_vein()
{
	for (int layer = 0; layer < this->layerNum; layer++) {
		vector<Ratio_Point> localRP = this->point_with_ratio[layer];

		for (int i = 0; i < (int)localRP.size(); i++) {
			if (localRP[i].veinIndex >= 0) {
				Point p = this->vein[localRP[i].veinIndex].get_point(localRP[i].ratio_x);
				this->v_list[localRP[i].index].set(p.x, p.y, p.z);
			}
		}
	}
	   
	//left egde
	if (this->leftVeinEgde.size() > 2) {

		for (int i = 0; i < (int)this->leftVeinEgde.size() - 1; i++) {
			float StartX, StartY, StartZ, EndX, EndY, EndZ;
			int startIndex = this->leftVeinEgde[i];
			int endIndex = this->leftVeinEgde[i + 1];
			int startLayer = this->index_to_layer(startIndex);
			int endLayer = this->index_to_layer(endIndex);

			StartX = this->v_list[startIndex].x;
			StartY = this->v_list[startIndex].y;
			StartZ = this->v_list[startIndex].z;
			EndX = this->v_list[endIndex].x;
			EndY = this->v_list[endIndex].y;
			EndZ = this->v_list[endIndex].z;

			for (int layer = startLayer + 1; layer < endLayer; layer++) {
				int index = this->layer_index[layer];
				float ratio_x = this->point_with_ratio[layer][0].ratio_x;
				float ratio_y = this->point_with_ratio[layer][0].ratio_y;
				float ratio_z = this->point_with_ratio[layer][0].ratio_z;

				float x = StartX + ratio_x*(EndX - StartX);
				float y = StartY + ratio_y*(EndY - StartY);
				float z = StartZ + ratio_z*(EndZ - StartZ);

				this->v_list[index].set(x, y, z);
			}
		}
	}

	//right egde
	if (this->rightVeinEgde.size() > 2) {

		for (int i = 0; i < (int)this->rightVeinEgde.size() - 1; i++) {
			float StartX, StartY, StartZ, EndX, EndY, EndZ;
			int startIndex = this->rightVeinEgde[i];
			int endIndex = this->rightVeinEgde[i + 1];
			int startLayer = this->index_to_layer(startIndex);
			int endLayer = this->index_to_layer(endIndex);

			StartX = this->v_list[startIndex].x;
			StartY = this->v_list[startIndex].y;
			StartZ = this->v_list[startIndex].z;
			EndX = this->v_list[endIndex].x;
			EndY = this->v_list[endIndex].y;
			EndZ = this->v_list[endIndex].z;

			for (int layer = startLayer + 1; layer < endLayer; layer++) {
				int index = this->layer_index[layer + 1] - 1;
				float ratio_x = this->point_with_ratio[layer].back().ratio_x;
				float ratio_y = this->point_with_ratio[layer].back().ratio_y;
				float ratio_z = this->point_with_ratio[layer].back().ratio_z;

				float x = StartX + ratio_x*(EndX - StartX);
				float y = StartY + ratio_y*(EndY - StartY);
				float z = StartZ + ratio_z*(EndZ - StartZ);

				this->v_list[index].set(x, y, z);
			}
		}
	}

	for (int layer = 0; layer < this->layerNum; layer++) {
		vector<Ratio_Point> localRP = this->point_with_ratio[layer];

		for (int i = 0; i < (int)localRP.size() - 1; i++) {
			int startIndex, endIndex;
			float StartX, StartY, StartZ, EndX, EndY, EndZ;
			startIndex = localRP[i].index;
			endIndex = localRP[i + 1].index;

			StartX = this->v_list[startIndex].x;
			StartY = this->v_list[startIndex].y;
			StartZ = this->v_list[startIndex].z;
			EndX = this->v_list[endIndex].x;
			EndY = this->v_list[endIndex].y;
			EndZ = this->v_list[endIndex].z;

			for (int index = startIndex + 1; index < endIndex; index++) {
				float x, y, z, ratio;

				ratio = (float)(index - startIndex) / (endIndex - startIndex);
				x = StartX + ratio*(EndX - StartX);
				y = StartY + ratio*(EndY - StartY);
				z = StartZ + ratio*(EndZ - StartZ);

				this->v_list[index].set(x, y, z);
			}
		}
	}


	for (int i = this->layer_index[this->layerNum - 1]; i < this->layer_index[this->layerNum]; i++) {
		this->v_list[i].z = this->mainvein.ctrlPoints[3].z;
	}

	return 1;
}

Leaf::Leaf(const vector<vector<float>> &vvv, float distance, vector<BezierLine> v)
{
	this->vein = v;
	this->veinNum = v.size();
	this->layer_distance = distance;


	gen_vertices_and_index(this->v_list, this->v_index, this->layer_index, vvv, distance);
	this->layerNum = layer_index.size() - 1;

	vector<vector<Ratio_Point>> pr(this->layerNum);
	this->point_with_ratio = pr;

	this->texGround = load_texture("texture.bmp");

	this->setup_layer_mid_index();
	this->modify_mesh_vertex_to_vein();
	this->cal_normal();
}

int Leaf::set(const vector<vector<float>> &vvv, float distance, vector<BezierLine> v)
{
	this->vein = v;
	this->veinNum = v.size();
	this->layer_distance = distance;


	gen_vertices_and_index(this->v_list, this->v_index, this->layer_index, vvv, distance);
	this->layerNum = layer_index.size() - 1;

	vector<vector<Ratio_Point>> pr(this->layerNum);
	this->point_with_ratio = pr;

	this->texGround = load_texture("texture.bmp");

	this->setup_layer_mid_index();
	this->modify_mesh_vertex_to_vein();
	this->cal_normal();

	return 1;
}


int Leaf::set(const vector<vector<float>> &vvv, float distance)
{
	this->layer_distance = distance;


	gen_vertices_and_index(this->v_list, this->v_index, this->layer_index, vvv, distance);
	this->layerNum = layer_index.size() - 1;

	vector<vector<Ratio_Point>> pr(this->layerNum);
	this->point_with_ratio = pr;

	this->texGround = load_texture("texture.bmp");

	this->setup_layer_mid_index();

	return 1;
}

int Leaf::setup()
{
	this->deform_with_mainvein();
	this->deform_with_vein();

	for (auto &v : this->vein) {
		v.refresh();
	}
	this->mainvein.refresh();

	//this->mesh.set(this->v_list, this->v_index);
	this->cal_normal();
	return 1;
}

int Leaf::draw()
{
	glPushMatrix();


	this->draw_edge();

	Point p;
	glLineWidth(1);
	glColor3f(1, 1, 1);

	if (this->texture_on == 1) {
		glEnable(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, this->texGround);
	}

	glBegin(GL_TRIANGLES);
	for (Triangle &t : this->v_index) {

			glTexCoord2f(0.0f, 0.0f);
			p = this->v_list[t.v1];
			glNormal3f(p.nx, p.ny, p.nz);
			glVertex3f(p.x, p.y, p.z);
			glTexCoord2f(0.0f, 1.0f); 
			p = this->v_list[t.v2];
			glNormal3f(p.nx, p.ny, p.nz);
			glVertex3f(p.x, p.y, p.z);
			glTexCoord2f(1.0f, 0.0f); 
			p = this->v_list[t.v3];
			glNormal3f(p.nx, p.ny, p.nz);
			glVertex3f(p.x, p.y, p.z);

	}
	glEnd();
	glDisable(GL_TEXTURE_2D);



	for (auto &v : this->vein) {
		v.draw();

	}
	this->mainvein.draw();




	glPopMatrix();
	return 1;
}


int Leaf::cal_normal() {
	for (Point &p : this->v_list) {
		p.set_normal(0, (layerNum)*layer_distance, 0);
	}

	return 1;
}


int Leaf::cal_vein_from_2point(vector<vector<Point>> startAndEndofVein)
{
	this->veinNum = startAndEndofVein.size();
	this->vein.clear();
	this->veinStartIndex.clear();
	
	Point start, end, veinStart, veinEnd, veinCtrlP1, veinCtrlP2;
	BezierLine v;
	int startLayer;
	float slope;
	for (int i = 0; i < this->veinNum; i++) {
		start = startAndEndofVein[i][0];
		end = startAndEndofVein[i][1];


		//start.y -= (this->layerNum*this->layer_distance) / 2;
		//end.y -= (this->layerNum*this->layer_distance) / 2;

		if (start.y >= 0 || start.y < this->v_list.back().y)
			continue;

		if (end.y >= 0 || end.y < this->v_list.back().y)
			continue;


		startLayer = abs((int)start.y / (int)this->layer_distance);

		veinStart = this->v_list[this->layer_mid_index[startLayer]];
		this->veinStartIndex.push_back(this->layer_mid_index[startLayer]);
		slope = (end.y - veinStart.y) / (end.x - veinStart.x);

		for (int layer = 0; layer < this->layerNum; layer++) {
			int tmpIndex;
			Point tmpP;
			float tmpSlope;
			if (slope < 0)
				tmpIndex = this->layer_index[layer];
			else
				tmpIndex = this->layer_index[layer + 1] - 1;

			tmpP = this->v_list[tmpIndex];
			tmpSlope = (tmpP.y - veinStart.y) / (tmpP.x - veinStart.x);
			if (abs(tmpSlope) < abs(slope)) {
				veinEnd = v_list[tmpIndex];
				break;
			}
		}

		veinCtrlP1.set((2 * veinStart.x + veinEnd.x) / 3, (2 * veinStart.y + veinEnd.y) / 3, 0);
		veinCtrlP2.set((veinStart.x + 2 * veinEnd.x) / 3, (veinStart.y + 2 * veinEnd.y) / 3, 0);

		vector<Point> tmpvp{ veinStart ,veinCtrlP1 ,veinCtrlP2 ,veinEnd };
		v.set(tmpvp);
		this->vein.push_back(v);
	}

	this->veinNum = this->vein.size();

	this->setup_layer_mid_index();
	this->modify_mesh_vertex_to_vein();
	this->cal_normal();

	return 1;

}


int Leaf::draw_edge() 
{
	Point *p;
	glBegin(GL_LINE_STRIP);
	glLineWidth(1);
	glColor3f(0.7, 0.7, 0);

	for (int layer = 0; layer < this->layerNum; layer++) {
		p = &(this->v_list[this->layer_index[layer]]);
		glVertex3f(p->x, p->y, p->z);
	}
	for (int layer = layerNum; layer > 0; layer--) {
		p = &(this->v_list[this->layer_index[layer]-1]);
		glVertex3f(p->x, p->y, p->z);
	}
	glEnd();

	return 1;
}

