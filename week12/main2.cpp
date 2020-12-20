//#include<opencv2/opencv.hpp>
//#include<iostream>
//using namespace cv;
//
//
//int Gaussianback(std::vector<cv::Mat> srcMats, cv::Mat& meanMat, cv::Mat& varMat) {
//	int height = srcMats[0].rows;//行数
//	int width = srcMats[0].cols;//每行元素的总元素量
//	int sum = 0;
//	float var = 0;
//	for (int j = 0; j < height; j++) {
//		for (int i = 0; i < width; i++) {
//			for (int i = 0; i < srcMats.size(); i++) {
//				sum += srcMats[i].at<int>(j, i);
//			}
//			meanMat.at<int>(j, i) = sum / srcMats.size();
//		}
//	}
//
//	for (int j = 0; j < height; j++) {
//		for (int i = 0; i < width; i++) {
//			for (int cnt = 0; cnt < srcMats.size(); cnt++) {
//				var += pow((srcMats[cnt].at<int>(j, i) - meanMat.at<int>(j, i)), 2);
//			}
//			varMat.at<float>(j, i) = var / srcMats.size();
//		}
//	}
//	return 0;
//}
//
//
//int GaussianThreshold(cv::Mat srcMat, cv::Mat meanMat, cv::Mat varMat, float a, cv::Mat& dstMat) {
//	int now;
//	int mean;
//	int height = srcMat.rows;
//	int width = srcMat.cols;
//	for (int j = 0; j < height; j++) {
//		for (int i = 0; i < width; i++) {
//			now = srcMat.at<int>(j, i);
//			mean = meanMat.at<int>(j, i);
//			int difference = abs(now - mean);
//			int th = a * varMat.at<float>(j, i);
//			if (difference > th) {
//				dstMat.at<uchar>(j, i) = 255;
//			}
//			else {
//				dstMat.at<uchar>(j, i) = 0;
//			}
//		}
//	}
//return 0;
//}
//int main() {
//	VideoCapture cap;
//	cap.open(0);
//	std::vector<cv::Mat> srcMats;
//	int nBg = 20;					
//	float a = 1;					
//	int cnt = 0;
//	cv::Mat frame;
//	cv::Mat meanMat;
//	cv::Mat varMat;
//	cv::Mat dstMat;
//
//
//
//	if (!cap.isOpened())
//	{
//		std::cout << "不能打开视频文件" << std::endl;
//		return -1;
//
//	}
//
//	while (1) {
//		cap >> frame;
//		cvtColor(frame, frame, COLOR_BGR2GRAY);
//		if (cnt < nBg) {
//			srcMats.push_back(frame);
//			if (cnt == 0) {
//				std::cout << "reading frame " << std::endl;
//						}
//		}
//		else if (cnt == nBg) {
//
//			meanMat.create(frame.size(), CV_8UC1);
//			varMat.create(frame.size(), CV_32FC1);
//			std::cout << "calculating background models" << std::endl;
//			Gaussianback(srcMats, meanMat, varMat);
//		}
//		else {//背景差分
//			dstMat.create(frame.size(), CV_8UC1);
//			GaussianThreshold(frame, meanMat, varMat, a, dstMat);
//			imshow("result", dstMat);
//			imshow("frame", frame);
//			waitKey(30);
//		}
//		cnt++;
//	}
//	return 0;
//}



#include<opencv2/opencv.hpp>
#include<iostream>

using namespace std;
using namespace cv;

int calcGaussianBackground(std::vector<cv::Mat> srcMats, cv::Mat& meanMat, cv::Mat& varMat)
{
	int rows = srcMats[0].rows;
	int cols = srcMats[0].cols;
	for (int h = 0; h < rows; h++)
	{
		for (int w = 0; w < cols; w++)
		{
			int sum = 0;
			float var = 0;
			for (int i = 0; i < srcMats.size(); i++) {
				sum += srcMats[i].at<uchar>(h, w);
			}
			meanMat.at<uchar>(h, w) = sum / srcMats.size();//均值
			for (int i = 0; i < srcMats.size(); i++) {
				var += pow((srcMats[i].at<uchar>(h, w) - meanMat.at<uchar>(h, w)), 2);
			}
			varMat.at<float>(h, w) = var / srcMats.size();//方差
		}
	}
	return 0;
}

int gaussianThreshold(cv::Mat srcMat, cv::Mat meanMat, cv::Mat varMat, float weight, cv::Mat& dstMat)
{
	int srcI;
	int meanI;
	int dstI;
	int rows = srcMat.rows;
	int cols = srcMat.cols;

	for (int h = 0; h < rows; h++)
	{
		for (int w = 0; w < cols; w++)
		{
			srcI = srcMat.at<uchar>(h, w);
			meanI = meanMat.at<uchar>(h, w);
			int dif = abs(srcI - meanI);
			int th = weight * varMat.at<float>(h, w);
			if (dif > th) {
				dstMat.at<uchar>(h, w) = 255;
			}
			else {
				dstMat.at<uchar>(h, w) = 0;
			}
		}
	}
	return 0;
}

int main()
{
	VideoCapture capVideo(0);
	if (!capVideo.isOpened()) {//打开失败
		std::cout << "can not open" << std::endl;
		return -1;
	}

	std::vector<cv::Mat> srcMats;	//背景模型图像
	int nBg = 200;					//背景模型数量
	float wVar = 1;					//方差权重
	int cnt = 0;
	cv::Mat frame;
	cv::Mat meanMat;
	cv::Mat varMat;
	cv::Mat dstMat;

	while (true)
	{
		capVideo >> frame;
		cvtColor(frame, frame, COLOR_BGR2GRAY);
		if (cnt < nBg) {//计算背景
			srcMats.push_back(frame);
			if (cnt == 0) {
				std::cout << "reading frame " << std::endl;
			}
		}
		else if (cnt == nBg) {//计算模型	
			meanMat.create(frame.size(), CV_8UC1);
			varMat.create(frame.size(), CV_32FC1);
			std::cout << "calculating background models" << std::endl;
			calcGaussianBackground(srcMats, meanMat, varMat);
		}
		else {//背景差分
			dstMat.create(frame.size(), CV_8UC1);
			gaussianThreshold(frame, meanMat, varMat, wVar, dstMat);
			imshow("result", dstMat);
			imshow("frame", frame);
			waitKey(30);
		}
		cnt++;
	}
	return 0;
}