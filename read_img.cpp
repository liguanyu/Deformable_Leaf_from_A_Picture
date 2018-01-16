#include "read_img.h"
using namespace cv;
using namespace std;

const int my_threshold = 256;



int sharpening_img(const char *filename, Mat &grad)
{

	Mat src, src_gray;

	int scale = 1;
	int delta = 0;
	int ddepth = CV_16S;


	src = cvLoadImage(filename, IMREAD_COLOR);

	if (src.empty())
	{
		return -1;
	}

	GaussianBlur(src, src, Size(3, 3), 0, 0, BORDER_DEFAULT);

	cvtColor(src, src_gray, COLOR_BGR2GRAY);

	Mat grad_x, grad_y;
	Mat abs_grad_x, abs_grad_y;

	Sobel(src_gray, grad_x, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT);

	Sobel(src_gray, grad_y, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT);

	convertScaleAbs(grad_x, abs_grad_x);
	convertScaleAbs(grad_y, abs_grad_y);

	addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad);



	return 0;
}



vector<float> get_vector_from_img(const char *filename, int step, int &h)
{

	Mat grad;
	IplImage *g_pSrcImage, *g_pCannyImg;

	if (sharpening_img(filename, grad) == -1) {
		cout << "read error" << endl;
	}

	g_pSrcImage = &IplImage(grad);

	g_pCannyImg = cvCreateImage(cvGetSize(g_pSrcImage), IPL_DEPTH_8U, 1);
	g_pSrcImage = &IplImage(grad);
	cvCanny((CvArr*)g_pSrcImage, (CvArr*)g_pCannyImg, my_threshold, my_threshold * 3, 3);

	vector<float> result;

	uchar* pixel = new uchar;
	for (int i = 0; i < g_pCannyImg->height; i += step)
	{
		int start, end;
		for (start = 0; start < g_pCannyImg->width; start++)
		{
			pixel = (uchar*)(g_pCannyImg->imageData + i*g_pCannyImg->widthStep + start);
			if (*pixel > 200) {

				break;
			}
		}

		if (start > g_pCannyImg->width - 5) {
			continue;
		}

		for (end = g_pCannyImg->width - 1; end >= 0; end--)
		{
			pixel = (uchar*)(g_pCannyImg->imageData + i*g_pCannyImg->widthStep + end);

			if (*pixel > 200) {
				break;
			}
		}

		result.push_back(end - start);
	}

	h = (int)(g_pCannyImg->height) / (int)result.size() + 1;


	return result;
}
