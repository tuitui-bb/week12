//#include<opencv2/opencv.hpp>
//#include<iostream>
//using namespace cv;
//int main() {
//	VideoCapture cap;
//	cap.open(0);
//	cv::Mat frame;
//	cv::Mat bgMat;
//	cv::Mat subMat;
//	cv::Mat bny_subMat;
//	int cnt = 0;
//	double fps = cap.get(CAP_PROP_FPS);
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
//		
//			if (cnt == 0) {
//				frame.copyTo(bgMat);
//			}
//			
//			else {
//
//				absdiff(frame, bgMat, subMat);
//				threshold(subMat, bny_subMat, 50, 255, CV_THRESH_BINARY);
//
//				imshow("bny_subMat", bny_subMat);
//				imshow("subMat", subMat);
//				waitKey(30);
//			}
//			cnt++;
//		}
//	
//	return 0;
//}