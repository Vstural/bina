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
	//row col start from 0
	return mat.data[row*mat.cols+col];
}

void setPixelValue(Mat& mat, int row, int col, int value)
{
	mat.data[row*mat.cols + col] = value;	
	//cout << value << "  " << int(mat->data[row*mat->cols + col]) << endl;
}

// require mat1 mat2 result have same rows and cols
//only keep the pixel that both exist
int myCombine(Mat &mat1, Mat &mat2,Mat& result)
{
	for (int i = 0; i < mat1.rows; i++)
	{
		for (int j = 0; j < mat1.cols; j++)
		{
			if (getIntPixelValue(mat1, i, j) && getIntPixelValue(mat2, i, j))
			{
				setPixelValue(result, i, j, 255);
			}
			else
			{
				setPixelValue(result, i, j, 0);
			}
		}
	}
	return 0;
}


int myCombineOR(Mat &mat1, Mat &mat2, Mat& result)
{
	for (int i = 0; i < mat1.rows; i++)
	{
		for (int j = 0; j < mat1.cols; j++)
		{
			if (getIntPixelValue(mat1, i, j) || getIntPixelValue(mat2, i, j))
			{
				setPixelValue(result, i, j, 255);
			}
			else
			{
				setPixelValue(result, i, j, 0);
			}
		}
	}
	return 0;
}

void reserve(Mat &mat)
{
	for (int i = 0; i < mat.rows - 1; i++)
	{
		for (int j = 0; j < mat.cols - 1; j++)
		{
			if (getIntPixelValue(mat, i, j) > 0)
			{
				setPixelValue(mat, i, j, 0);
			}
			else
			{
				setPixelValue(mat, i, j, 255);
			}
		}
	}
}

void removeSinglePixel(Mat &mat)
{
	for (int i = 1; i < mat.rows - 1; i++)
	{
		for (int j = 1; j < mat.cols - 1; j++)
		{
			if (getIntPixelValue(mat, i - 1, j) == 0 &&
				getIntPixelValue(mat, i, j - 1) == 0 &&
				getIntPixelValue(mat, i + 1, j) == 0 &&
				getIntPixelValue(mat, i,j + 1) == 0)
			{
				setPixelValue(mat, i, j, 0);
			}
		}
	}
}

void initMat(Mat &mat)
{
	for (int i = 0; i < mat.rows; i++)
	{
		for (int j = 0; j < mat.cols; j++)
		{
			setPixelValue(mat, i, j, 0);
		}
	}
}

void connect(Mat &mat,Mat &res)
{
	for (int i = 1; i < mat.rows - 1; i++)
	{
		for (int j = 1; j < mat.cols - 1; j++)
		{
			if (getIntPixelValue(mat, i - 1, j) && getIntPixelValue(mat, i + 1, j))
			{
				setPixelValue(res, i, j, 250);
			}
			if (getIntPixelValue(mat, i, j - 1) && getIntPixelValue(mat, i, j + 1))
			{
				setPixelValue(res, i, j, 250);
			}
		}
	}
	myCombineOR(mat, res, res);
}

int getFontWidth(Mat mat) {
	int* width = new int[mat.cols+1]();

	for (int i = 0; i < mat.rows; i++) 
	{
		int cur = 0;
		for (int j = 0; j < mat.cols; j++)
		{
			cur++;
			if (getIntPixelValue(mat, i, j) != 0)
			{
				width[cur] += 1;
				//cout << "!!!++== : " << cur << "   " << *(width+cur)<< endl;
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
	delete [] width;
	return target;
}

int main()
{
	//Mat src = imread("docexp2.jpg");
	Mat src = imread("test2.jpg");

	if (src.empty())
	{
		cout << "get image error" << endl;
		return -1;
	}

	Mat gray;

	cvtColor(src, gray, CV_BGR2GRAY);
	//Canny
	Mat canny;
	Canny(gray, canny, 150, 60, 3);

	Mymat grayM(gray);
	//gradient and contrast
	Mymat fig2aM(gray);

	//get the windwos size by font width
	grayM.window = getFontWidth(canny);
	
	Mymat fig2bM(gray.rows, gray.cols);

	//for each pixel
	for (int row = grayM.window; row < gray.rows - grayM.window; row++)
	{
		for (int col = 1 + grayM.window; col < gray.cols - grayM.window; col++)
		{

			//do
			//for pixel sourround it within size of windows
			int min = 255, max = 0;
			for (int i = row - grayM.window; i < row + grayM.window; i++)
			{
				for (int j = col - grayM.window; j < col + grayM.window; j++)
				{
					if (grayM.getPixel(i, j) < min)
					{
						min = grayM.getPixel(i, j);
					}
					if (grayM.getPixel(i, j) > max)
					{
						max = grayM.getPixel(i, j);
					}
				}
			}
			fig2aM.setPixel(row, col, 
				max- min);
			fig2bM.setPixel(row, col, 
				(255*(max - min))
				/ (max + min + 0.01));
		}
		//cout << "LINE" << endl;
	}
	Mat fig2aMshow = gray.clone();
	fig2aM.getMat(fig2aMshow);

	Mat fig2bMshow = gray.clone();
	fig2bM.getMat(fig2bMshow);

	//C alpha
	Mymat Calpha(gray.rows, gray.cols);
	int alpha = 0.95;
	for (int i = grayM.window; i < gray.rows - grayM.window; i++)
	{
		for (int j = 1 + grayM.window; j < gray.cols - grayM.window; j++)
		{
			Calpha.setPixel(i, j, alpha*fig2aM.getPixel(i, j) + (1-alpha)*fig2bM.getPixel(i, j));
		}
	}
	Mat Calphashow = gray.clone();
	Calpha.getMat(Calphashow);

	//Otsu's
	Mat otsu;
	threshold(Calphashow, otsu, 0, 255, THRESH_OTSU);

	//Combine
	Mat comb = otsu.clone();
	//myCombine(canny, otsu, comb);
	removeSinglePixel(comb);
	Mat connectRes = gray.clone();
	initMat(connectRes);
	connect(comb, connectRes);
	imshow("1", otsu);
	imshow("2", comb);
	imshow("3", connectRes);
	//Ger result
	Mymat res(connectRes);
	grayM.window = 1;

	Mat resshow = gray.clone();
	res.getMat(resshow);
	
	for (int i = grayM.window; i < grayM.getrows()- grayM.window; i++)
	{
		for (int j = grayM.window; j < grayM.getcols()- grayM.window; j++)
		{

			if (grayM.getPixel(i, j) <= (res.getMeanAverage(i, j) + (res.getStdAverage(i, j) / 2)))
			{
				setPixelValue(resshow, i, j, 255);
			}
			else
			{
				setPixelValue(resshow, i, j, 0);
			}
		}
	}

	//removeSinglePixel(resshow);
	Mat resshow2 = resshow.clone();

	//the only different between res1 and res2,resshow2 is after connect
	connect(resshow, resshow2);
	removeSinglePixel(resshow2);
	///
	reserve(resshow);
	reserve(resshow2);
	imshow("res1show",resshow);
	imshow("resshow2", resshow2);

	//Mat otsu2;
	//threshold(gray, otsu2, 0, 255, THRESH_OTSU);
	//imshow("byOTSU", otsu2);
	imwrite("res1.jpg", resshow);
	imwrite("res2.jpg", resshow2);
	waitKey();
	return 0;
}
