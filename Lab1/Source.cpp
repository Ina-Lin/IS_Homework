#include <cstdio>
#include <opencv2/opencv.hpp>
#include <math.h>
using namespace cv;

int main(){
	Mat lena = imread("C:\\lena.jpg");
	Mat out;
	vector<Mat> mat(3);
	vector<Mat> mat1(6);
	int h = lena.rows;
	int w = lena.cols;
	cvtColor(lena, out, CV_BGR2HSV);
	split(out, mat);

	mat1[0] = mat[0].clone();
	mat1[1] = mat[1].clone();
	mat1[2] = mat[2].clone();
	mat1[3] = mat[0].clone();
	mat1[4] = mat[1].clone();
	mat1[5] = mat[2].clone();

	for (int i = 0; i < w; i++){
		for (int j = 0; j < h; j++){
			int log = (int)(32 * log2(mat[0].at<uchar>(j, i) + 1));
			int inv = (int)(pow(2, mat[0].at<uchar>(j, i) / 32.0) - 1);
			if (log > 255)log = 255;
			if (inv > 255)inv = 255;
			mat1[0].at<uchar>(j, i) = log;
			mat1[1].at<uchar>(j, i) = log;
			mat1[2].at<uchar>(j, i) = log;
			mat1[3].at<uchar>(j, i) = inv;
			mat1[4].at<uchar>(j, i) = inv;
			mat1[5].at<uchar>(j, i) = inv;
		}
	}

	imwrite("H.jpg", mat[0]);
	imwrite("S.jpg", mat[1]);
	imwrite("V.jpg", mat[2]);
	imwrite("H-log.jpg", mat1[0]);
	imwrite("S-log.jpg", mat1[1]);
	imwrite("V-log.jpg", mat1[2]);
	imwrite("H-inverse-log.jpg", mat1[3]);
	imwrite("S-inverse-log.jpg", mat1[4]);
	imwrite("V-inverse-log.jpg", mat1[5]);

	waitKey(0);

	return 0;
}