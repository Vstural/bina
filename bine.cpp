#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <typeinfo>
#include "Mymat.h"
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>

using namespace cv;
using namespace std;

int getIntPixelValue(Mat mat, int row, int col)
{
	return mat.data[row*mat.cols + col];
}

void setPixelValue(Mat& mat, int row, int col, int value)
{
	mat.data[row*mat.cols + col] = value;
}

int getFontWidth(Mat mat) {
	int* width = new int[mat.cols + 1]();

	for (int i = 0; i < mat.rows; i++)
	{
		int cur = 0;
		for (int j = 0; j < mat.cols; j++)
		{
			cur++;
			if (getIntPixelValue(mat, i, j) != 0)
			{
				width[cur] += 1;
				cur = 0;
			}
		}
	}
	int target = 0;
	int maxNum = 0;
	for (int i = 0; i < mat.cols + 1; i++)
	{
		if (*(width + i) > maxNum)
		{
			maxNum = *(width + i);
			target = i;
		}
	}
	delete[] width;
	return target;
}

int main(int argc, char * argv[])
{
	Mat src = imread(argv[1],0);

	if (src.empty())
	{
		cout << "get image error" << endl;
		return -1;
	}


	Mat cannyMat;
	Canny(src, cannyMat, 50, 200);
	
	int windows = getFontWidth(src);
	cout << windows << endl;
	Mat res(src.rows, src.cols, CV_8UC1);

	imshow("src", src);
	imshow("res", res);

	waitKey(0);
	return 0;
}