#include "control_functions.h"

int DEFORM_ON = 0;

bool VEIN_BUILD_DONE = 0;

const GLfloat PI = 3.1415f;
GLfloat xRot = 0.0f;
GLfloat yRot = 0.0f;

Leaf leaf;
vector<vector<Point>> mouseP;


struct Chosen_point {
	int vein;
	int index;
}chosen_point;
int CHOOSE_MODE = 0;


int h = 10;
int layer_ratio = 2;

static bool comp_vector_point(vector<Point> &a, vector<Point> &b);
static void get_point_window_coordinate(Point p, GLdouble &wx, GLdouble &wy);
static void get_cloest_point(int x, int y, int &vein, int &index);
static void auto_deform();
static void output_key_information();

void initial_lgy_project() 
{
	DEFORM_ON = 0;
	VEIN_BUILD_DONE = 0;
	xRot = 0.0f;
	yRot = 0.0f;
	mouseP.clear();
	CHOOSE_MODE = 0;

	vector<vector<float>> vvv;

	vector<float> mmm = get_vector_from_img("leaf_img.jpg", 15, h);

	h /= layer_ratio;

	vvv = generate_vertices(mmm, (int)mmm.size()* layer_ratio);

	leaf.set(vvv, h);
	glutDisplayFunc(rendererScene);

	float height = 2 * (leaf.layerNum / 2)*leaf.layer_distance;
	float width = height;

	glOrtho(0.6*width, -0.6*width, -0.6*height, 0.6*height, -3 * height, 3 * height);
}



void rendererScene(void)
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  
																		
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glPushMatrix();
	glRotatef(xRot, 1.0f, 0.0f, 0.0f);
	glRotatef(yRot, 0.0f, 1.0f, 0.0f);
	glTranslatef(0, (leaf.layerNum / 2)*leaf.layer_distance, 0);

	if(leaf.texture_on == 0)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


	if (!VEIN_BUILD_DONE) {
		glLineWidth(3);
		glColor3f(1, 1, 1);
		glBegin(GL_LINES);
		for (int i = 0; i < mouseP.size(); i++) {
			if (mouseP[i].size() == 2) {
				glVertex3f(mouseP[i][0].x, mouseP[i][0].y, mouseP[i][0].z);
				glVertex3f(mouseP[i][1].x, mouseP[i][1].y, mouseP[i][1].z);
			}
		}

		glEnd();
	}


	leaf.draw();

	if (CHOOSE_MODE) {
		Point HightLightP;
		if (chosen_point.vein == -2) {
			HightLightP = leaf.mainvein.ctrlPoints[chosen_point.index];
			leaf.mainvein.draw_ctrl_points();
		}
		else {
			HightLightP = leaf.vein[chosen_point.vein].ctrlPoints[chosen_point.index];
			leaf.vein[chosen_point.vein].draw_ctrl_points();
		}

		glDisable(GL_DEPTH_TEST);
		glPointSize(10);
		glColor3f(1, 0, 0);
		glBegin(GL_POINTS);
		glVertex3f(HightLightP.x, HightLightP.y, HightLightP.z);
		glEnd();
	}


	glPopMatrix();



	glutSwapBuffers();
}



void setupRC(void)
{
	//背景颜色
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glShadeModel(GL_FLAT);

	glFrontFace(GL_CW);

}

//rotate func
void rotateMode(int key, int x, int y)
{
	if (key == GLUT_KEY_UP)
		xRot -= 5.0f;

	else if (key == GLUT_KEY_DOWN)
		xRot += 5.0f;

	else if (key == GLUT_KEY_LEFT)
		yRot -= 5.0f;

	else if (key == GLUT_KEY_RIGHT)
		yRot += 5.0f;

	if (xRot < 0) xRot = 355.0f;
	if (xRot > 360.0f) xRot = 0.0f;

	if (yRot < 0) yRot = 355.0f;
	if (yRot > 360.0f) yRot = 0.0f;



	glutPostRedisplay();

}


//change Ctrl Point func
void choose_ctrlP_mode(unsigned char key, int x, int y)
{
	if (DEFORM_ON == 1) {
		if (chosen_point.vein == -2) {
			switch (key) {
			case 'w':
				leaf.mainvein.change_ctrlP(chosen_point.index, 2, -10);
				break;
			case 's':
				leaf.mainvein.change_ctrlP(chosen_point.index, 2, 10);
				break;
			case 'a':
				leaf.mainvein.change_ctrlP(chosen_point.index, 1, -10);
				break;
			case 'd':
				leaf.mainvein.change_ctrlP(chosen_point.index, 1, 10);
				break;
			case 'q':
				leaf.mainvein.change_ctrlP(chosen_point.index, 3, 10);
				break;
			case 'e':
				leaf.mainvein.change_ctrlP(chosen_point.index, 3, -10);
				break;
			default:
				break;
			}
		}
		else if (chosen_point.vein < leaf.veinNum) {
			switch (key) {
			case 'w':
				leaf.vein[chosen_point.vein].change_ctrlP(chosen_point.index, 2, -10);
				break;
			case 's':
				leaf.vein[chosen_point.vein].change_ctrlP(chosen_point.index, 2, 10);
				break;
			case 'a':
				leaf.vein[chosen_point.vein].change_ctrlP(chosen_point.index, 1, -10);
				break;
			case 'd':
				leaf.vein[chosen_point.vein].change_ctrlP(chosen_point.index, 1, 10);
				break;
			case 'q':
				leaf.vein[chosen_point.vein].change_ctrlP(chosen_point.index, 3, 10);
				break;
			case 'e':
				leaf.vein[chosen_point.vein].change_ctrlP(chosen_point.index, 3, -10);
				break;
			default:
				break;
			}
		}
	}

	// change between texture and grid
	switch (key) {
	case 't':
		if (leaf.texture_on == 1)
			leaf.texture_on = 0;
		else
			leaf.texture_on = 1;
		break;
	case 'p':
		auto_deform();
		break;
	default:
		break;
	}
	leaf.setup();

	glutPostRedisplay();
}


void myMouse(int button, int state, int x, int y)
{
	GLint viewport[4];
	GLdouble mvmatrix[16], projmatrix[16];
	GLint realy;
	GLdouble wx, wy, wz;

	static bool judge = 0;

	// select points to build vein 
	if (VEIN_BUILD_DONE == 0) {
		switch (button) {
		case GLUT_LEFT_BUTTON:
			if (state == GLUT_DOWN) {

				glPushMatrix();
				glRotatef(xRot, 1.0f, 0.0f, 0.0f);
				glRotatef(yRot, 0.0f, 1.0f, 0.0f);
				glTranslatef(0, (leaf.layerNum / 2)*leaf.layer_distance, 0);

				glGetIntegerv(GL_VIEWPORT, viewport);
				glGetDoublev(GL_MODELVIEW_MATRIX, mvmatrix);
				glGetDoublev(GL_PROJECTION_MATRIX, projmatrix);
				realy = viewport[3] - y - 1;
				gluUnProject(x, realy, 0, mvmatrix, projmatrix, viewport, &wx,
					&wy, &wz);

				glPopMatrix();

				Point tmp(wx, wy, wz);

				if (judge == 0) {
					vector<Point> tmp_vector;
					mouseP.push_back(tmp_vector);
					mouseP.back().push_back(tmp);
					judge = 1;
				}
				else {
					if (tmp.y-20 < mouseP.back()[0].y)
						break;
					mouseP.back().push_back(tmp);
					judge = 0;
					glutPostRedisplay();
				}


			}break;
		//select done
		case GLUT_RIGHT_BUTTON:
			if (state == GLUT_DOWN)
			{
				sort(mouseP.begin(), mouseP.end(), comp_vector_point);
				leaf.cal_vein_from_2point(mouseP);
				leaf.setup();
				glutPostRedisplay();
				VEIN_BUILD_DONE = 1;
				DEFORM_ON = 1;
				output_key_information();
			}
		default: break;
		}
	}

	//choose the ctrl point to deform
	if (DEFORM_ON == 1) {
		switch (button) {
		case GLUT_LEFT_BUTTON:
			if (state == GLUT_DOWN) {

				printf("鼠标坐标为： (%4d, %4d)\n", x, y);

				get_cloest_point(x, y, chosen_point.vein, chosen_point.index);


				CHOOSE_MODE = 1;
				glutPostRedisplay();
			}
		}
	}
}

//less than
static bool comp_vector_point(vector<Point> &a, vector<Point> &b)
{
	if (a[1].x < 0 && b[1].x > 0)
		return true;
	else if (a[1].x > 0 && b[1].x < 0)
		return false;
	else
		return !(a[1].y < b[1].y);
}



static void get_point_window_coordinate(Point p, GLdouble &wx, GLdouble &wy)
{
	GLint viewport[4];
	GLdouble mvmatrix[16], projmatrix[16];
	GLdouble realy;
	GLdouble wz;

	glPushMatrix();
	glRotatef(xRot, 1.0f, 0.0f, 0.0f);
	glRotatef(yRot, 0.0f, 1.0f, 0.0f);
	glTranslatef(0, (leaf.layerNum / 2)*leaf.layer_distance, 0);

	glGetIntegerv(GL_VIEWPORT, viewport);
	glGetDoublev(GL_MODELVIEW_MATRIX, mvmatrix);
	glGetDoublev(GL_PROJECTION_MATRIX, projmatrix);

	gluProject(p.x, p.y, p.z,mvmatrix, projmatrix, viewport, &wx, &realy, &wz);
	wy = viewport[3] - realy - 1;

	glPopMatrix();
}

static void get_cloest_point(int x, int y, int &vein, int &index)
{
	double short_distance = -1;
	double distance = -1;
	double px, py;
	Point closestP;
	Point p;
	for (int i = 0; i < leaf.mainvein.ctrlPointNum; i++) {
		p = leaf.mainvein.ctrlPoints[i];
		get_point_window_coordinate(p, px, py);
		distance = (x - px)*(x - px) + (y - py)*(y - py);
		if (short_distance > distance || short_distance < 0) {
			short_distance = distance;
			vein = -2;
			index = i;
		}
	}

	for (int v = 0; v < leaf.veinNum; v++) {
		for (int i = 1; i < leaf.vein[v].ctrlPointNum; i++) {
			p = leaf.vein[v].ctrlPoints[i];
			get_point_window_coordinate(p, px, py);
			distance = (x - px)*(x - px) + (y - py)*(y - py);
			if (short_distance > distance || short_distance < 0) {
				short_distance = distance;
				vein = v;
				index = i;
			}
		}
	}

}

static void auto_deform()
{
	for (int i = 0; i < 144; i++) {
		leaf.mainvein.change_ctrlP(0, 3, 1.3/4);
		leaf.mainvein.change_ctrlP(3, 3, 1.3/4);

		for (int v = 0; v < leaf.veinNum; v++) {
			leaf.vein[v].change_ctrlP(1, 3, 1/4);
			leaf.vein[v].change_ctrlP(2, 3, 1.3/4);
			leaf.vein[v].change_ctrlP(3, 3, 3.0/4);
		}

		yRot += 10.0f/4;
		leaf.setup();
		rendererScene();
		Sleep(50);
	}
}

static void output_key_information()
{
	cout << "###########################" << endl;
	cout << "键盘：上下左右按钮可以旋转模型" << endl
		<< "在屏幕单击鼠标可以选择叶脉和主脉的控制点" << endl
		<< "此时键盘：w a s d键分别使控制点上下左右位移" << endl
		<< "q和e按钮使控制点在z方向上位移" << endl
		<< "p键进行自动预设变形" << endl
		<< "t键切换纹理映射与网格模型" << endl;
}