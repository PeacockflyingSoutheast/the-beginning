// 花盘.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include<iostream>
#include<opencv.hpp>
#include<vector>
#include<math.h>
using namespace std;
using namespace cv;
Mat frame, dst, grayImg, hsvImg;
vector<vector<Point>> contours;
vector<Vec4i> hierarchy;
vector<Mat>  hsvSplit;
int threshold_v = 170;//阈值的初始值
int threshold_max = 255;//阈值最大值
const char*output_win = "dst";
const char* source_win = "input frame";
void threshold_Callback(int, void*);
int main()
{
	VideoCapture cap(0);
	while (1)
	{
		cap >> frame;
		cvtColor(frame, hsvImg, CV_BGR2HSV);
		split(hsvImg, hsvSplit);//分离HSV为三个通道 H、S、V。	
		equalizeHist(hsvSplit[2], hsvSplit[2]);//提高颜色质量
		merge(hsvSplit, hsvImg);//合并分离的三通道
		inRange(hsvImg, Scalar(0, 43, 46), Scalar(10, 255, 255), dst);
		
		Mat element = getStructuringElement(MORPH_RECT, Size(3, 3));
		dilate(dst, dst, element);//膨胀
		erode(dst, dst, element);//腐蚀
		blur(dst, dst, Size(3, 3));//模糊
		//namedWindow("dst", CV_WINDOW_NORMAL);
		//createTrackbar("Threshold Value", output_win, &threshold_v, threshold_max,threshold_Callback);//创建滑动条控制阈值
		/*threshold_Callback(threshold_v, 0);*/
		findContours(dst, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));//寻找轮廓
		for (int i = 0; i < contours.size(); i++)
		{
			double tmparea = fabs(contourArea(contours[i]));

			if (tmparea > 10000 && tmparea<20000)//面积
			{
				rectangle(frame, boundingRect(contours.at(i)), Scalar(255, 100, 100), 2);
				putText(frame, "The flower", Point(frame.rows / 5, frame.cols / 5), FONT_HERSHEY_SIMPLEX, 2, Scalar(0, 255, 255), 5, 12);
			}
		}
		
		imshow("dst", frame);
		waitKey(1);
	}
	return 0;
}
void threshold_Callback(int, void*)//回调函数
{
	threshold(grayImg, dst, threshold_v, threshold_max, THRESH_OTSU);
	imshow("dst", frame);
}

