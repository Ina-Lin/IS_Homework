#include <opencv2/core/core.hpp> //basic
#include <opencv2/highgui/highgui.hpp> //imread or ¡K
#include <opencv2/nonfree/nonfree.hpp>  //sift or surf <- important
#include <opencv2/legacy/legacy.hpp>  //matcher

using namespace cv;
using namespace std;


int main(){
	Mat org_obj = imread("obj.jpg");
	Mat gray_obj = imread("obj.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	Mat org_scene = imread("scene.jpg");
	Mat gray_scene = imread("scene.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	Mat des_obj;
	Mat des_scene;
	Mat out_obj;
	Mat out_scene;
	Mat img_matches;
	vector<Point2f> src;
	vector<Point2f> target;
	vector<KeyPoint> keypoints1;
	vector<KeyPoint> keypoints2;
	vector< DMatch > matches;
	vector< DMatch > good_matches;
	BruteForceMatcher< L2<float> > matcher;
	
	SIFT sift(100, 3, 0.04, 10, 1.6f);
	sift(gray_obj, noArray(), keypoints1, des_obj, 0);
	sift(gray_scene, noArray(), keypoints2, des_scene, 0);

	drawKeypoints(org_obj, keypoints1, out_obj, Scalar(0, 0, 255), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
	drawKeypoints(org_scene, keypoints2, out_scene, Scalar(0, 0, 255), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
	
	matcher.match(des_obj, des_scene, matches);
	
	drawMatches(org_obj, keypoints1, org_scene, keypoints2, matches, img_matches, Scalar::all(-1), Scalar::all(-1), vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

	double min_dist = 1000;
	for (int i = 0; i < (int)matches.size(); i++) { // find the min_distance
		double dist = matches[i].distance;
		if (dist < min_dist)
			min_dist = dist;
	} 

	for (int i = 0; i < (int)matches.size(); i++) {
		if (matches[i].distance < 3 * min_dist) // you can chage the number "3 "
			good_matches.push_back(matches[i]);
	} // for


	for (int i = 0; i < good_matches.size(); i++) {
		src.push_back(keypoints1[good_matches[i].queryIdx].pt);
		target.push_back(keypoints2[good_matches[i].trainIdx].pt);
	}
	Mat H = findHomography(target, src, CV_RANSAC);
	Mat os;
	warpPerspective(org_scene, os, H, Size(org_obj.cols * 2, org_obj.rows));
	
	Mat target_in_big_mat(os, Rect(0, 0, org_obj.cols, org_obj.rows));
	org_obj.copyTo(target_in_big_mat);
	imshow("big_mat", os);
	imwrite("final.jpg", os);
//	imshow("os", os);
	waitKey(0);
}