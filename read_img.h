#ifndef READ_IMG_H
#define READ_IMG_H

#include <opencv2/opencv.hpp>  
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>


#include <vector>



//����ͼƬ������Ҷ�����������Ϣ
std::vector<float> get_vector_from_img(const char *filename, int step, int &h);

#endif // !READ_IMG_H
