#include<opencv2/opencv.hpp>
#include<iostream>

using namespace std;
using namespace cv;

int gaosijianmo(std::vector<cv::Mat> srcMats, cv::Mat& meanMat,cv::Mat& varMat)
{
	int rows = srcMats[0].rows;
	int cols = srcMats[0].cols;
	for (int j = 0; j < rows; j++)
	{
		for (int i = 0; i < cols; i++)
		{
			int sum = 0;
			float var = 0;
			for (int a = 0; a < srcMats.size(); a++) {
				sum += srcMats[a].at<uchar>(j, i);
			}
			meanMat.at<uchar>(j, i) = sum / srcMats.size();
			for (int a = 0; a < srcMats.size(); a++) {
				var += pow((srcMats[a].at<uchar>(j, i) - meanMat.at<uchar>(j, i)), 2);
			}
			varMat.at<float>(j, i) = var/srcMats.size();
		}
	}
	return 0;
}

int beijingchafen(cv::Mat srcMat, cv::Mat meanMat, cv::Mat varMat, float b, cv::Mat& dstMat)
{
	int srcI;
	int meanI;
	int dstI;
	int rows = srcMat.rows;
	int cols = srcMat.cols;

	for (int j = 0; j < rows; j++)
	{
		for (int i = 0; i < cols; i++)
		{
			srcI = srcMat.at<uchar>(j, i);
			meanI = meanMat.at<uchar>(j, i);
			int dif = abs(srcI - meanI);
			int th = b * varMat.at<float>(j, i);
			if (dif > th) {
				dstMat.at<uchar>(j, i) = 255;
			}
			else {
				dstMat.at<uchar>(j, i) = 0;
			}
		}
	}
	return 0;
}

int main()
{
	VideoCapture cap;
	cap.open(0);
	if (!cap.isOpened()) {//打开失败
		std::cout << "不能打开摄像头" << std::endl;
		return -1;
	}

	std::vector<cv::Mat> srcMats;	
	int n = 20;					
	float a = 1;					
	cv::Mat frame;
	cv::Mat meanMat;
	cv::Mat varMat;
	cv::Mat dstMat;
	int cnt = 0;

	while (1)
	{
		cap >> frame;
		cvtColor(frame, frame, COLOR_BGR2GRAY);
		if (cnt < n) {//计算背景
			srcMats.push_back(frame);
			if (cnt == 0) {
				std::cout << "reading frame " << std::endl;
			}
		}
		else if (cnt == n) {	
			meanMat.create(frame.size(), CV_8UC1);
			varMat.create(frame.size(), CV_32FC1);
			std::cout << "calculating background models" << std::endl;
			gaosijianmo(srcMats, meanMat, varMat);
		}
		else {//背景差分
			dstMat.create(frame.size(), CV_8UC1);
			beijingchafen(frame, meanMat, varMat, a, dstMat);
			imshow("result", dstMat);
			imshow("frame", frame);
			waitKey(30);
		}
		cnt++;
	}
	return 0;
}