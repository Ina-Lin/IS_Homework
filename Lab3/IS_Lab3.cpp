#include <cstdio>
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

int main(){
	VideoCapture stream("input.mp4");
	while (true) {
		Mat frame;
		Mat ycrcb;
		Mat range;
		Mat gauss;
		Mat out;
		Rect bounding_rect;
		stream >> frame;
		if (!(stream.read(frame))){
			break;
		}
		GaussianBlur(frame, gauss, Size(5, 5), 0, 0);
		cvtColor(gauss, ycrcb, CV_BGR2YCrCb);
		inRange(ycrcb, Scalar(0, 138, 100), Scalar(255, 179, 127), range); //126 127
		frame.copyTo(out, range);
		vector<vector<Point>> contours; // init contours
		findContours(range, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE); //calculate contours
		for (int i = 0; i < contours.size(); i++)
		{
			bounding_rect = boundingRect(contours[i]);
			drawContours(out, contours, i, Scalar(0, 0, 255));
			//draw contours[i] on image_gbr
			double n = contourArea(contours[i]);
			//calculate  area of contour
			ostringstream strs;
			strs << n;
			string str = "pixels ";
			str += strs.str();
			rectangle(out, bounding_rect, Scalar(255, 0, 0), 1, 8, 0);
			putText(out, str, Point(bounding_rect.x, bounding_rect.y), FONT_HERSHEY_COMPLEX, 1, Scalar(0, 0, 255));
		
		}
		if (stream.get(CV_CAP_PROP_POS_FRAMES) == 100){
			imwrite("100th.jpg", out);
		}
		else if (stream.get(CV_CAP_PROP_POS_FRAMES) == 200){
			imwrite("200th.jpg", out);
		}
		
		imshow("crcb", out);
		waitKey(30);
	}
}