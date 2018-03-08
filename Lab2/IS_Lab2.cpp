#include <cstdio>
#include <opencv2/opencv.hpp>
using namespace cv;

int main(){
	Mat element = getStructuringElement(MORPH_RECT, Size(3, 3));
	BackgroundSubtractorMOG2 MOG(55, 16);
	VideoCapture stream("video.avi");

	while (true) {
		Mat frame;
		Mat src;
		Mat fore;
		Mat out;

		stream >> frame;
		if (!(stream.read(frame))){
			break;
		}
		
		MOG.operator()(frame, fore);
		imshow("fore", fore);

		threshold(fore, src, 127, 255, THRESH_BINARY);
		erode(src, src, element);
		dilate(src, src, element);
		imshow("morph", src);

		frame.copyTo(out, src);
		imshow("mask", out);

		if (stream.get(CV_CAP_PROP_POS_FRAMES) == 150){
			imwrite("150origin.jpg", frame);
			imwrite("150foreground.jpg", fore);
			imwrite("105morphology.jpg", src);
			imwrite("150mask.jpg", out);
		}
		else if (stream.get(CV_CAP_PROP_POS_FRAMES) == 200){
			imwrite("200origin.jpg", frame);
			imwrite("200foreground.jpg", fore);
			imwrite("200morphology.jpg", src);
			imwrite("200mask.jpg", out);
		}
		waitKey(30);
	}
	return 0;
}