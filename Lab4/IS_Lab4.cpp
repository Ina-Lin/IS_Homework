#include <opencv2/objdetect/objdetect.hpp>
#include <cstdio>
#include <opencv2/opencv.hpp>
using namespace cv;
 
int main(){
	CascadeClassifier face_cascade;
	CascadeClassifier eye_cascade;

	vector<Rect> FACES;
	vector<Rect> EYES;

	double scaleFactor = 1.1;
	int minNeighbors = 3;
	int flags = 0;
	Size minSize = Size();
	Size maxSize = Size();
	Mat frame;
	Mat gray;

	
	if (!face_cascade.load("haarcascade_frontalface_alt.xml")){
		printf("--(!)Error loading\n");
		return -1;
	}

	if (!eye_cascade.load("haarcascade_eye_tree_eyeglasses.xml")){
		printf("--(!)Error loading\n");
		return -1;
	}

	
	VideoCapture stream("SRC.mp4");
	while (true) {
		
		stream >> frame;
		if (!(stream.read(frame))){
			break;
		}
		cvtColor(frame, gray, CV_BGR2GRAY);
		face_cascade.detectMultiScale(gray, FACES, scaleFactor, minNeighbors, flags, Size(123, 123));
	//	eye_cascade.detectMultiScale(gray, EYES, scaleFactor, minNeighbors, flags, Size(100, 100));
		
		for (int i = 0; i<FACES.size(); i++){
			rectangle(frame, FACES[i], Scalar(0, 255, 0), 4, 8, 0); // 
			Mat faceROI = gray(FACES[i]);

			//-- In each face, detect eyes
			eye_cascade.detectMultiScale(faceROI, EYES, scaleFactor, minNeighbors, flags, Size(10, 10));

			for (size_t j = 0; j < EYES.size(); j++)
			{
				Point center(FACES[i].x + EYES[j].x + EYES[j].width*0.5, FACES[i].y + EYES[j].y + EYES[j].height*0.5);
				int radius = cvRound((EYES[j].width + EYES[j].height)*0.25);
				circle(frame, center, radius, Scalar(255, 0, 0), 4, 8, 0);
			}
		}

		if (stream.get(CV_CAP_PROP_POS_FRAMES) == 100){
			imwrite("100th.jpg", frame);
		}
		else if (stream.get(CV_CAP_PROP_POS_FRAMES) == 300){
			imwrite("300th.jpg", frame);
		}

		imshow("crcb", frame);
		waitKey(30);

	}


}