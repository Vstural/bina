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
	myCombine(mat, res, res);
}

int main()
{
	//Mat src = imread("test.jpg");
	//Mat src = imread("sample.jpg");
	//Mat src = imread("EWtest2.jpg");
	//Mat src = imread("HW5.bmp");
	//Mat src = imread("partofPR8.bmp");
	//Mat src = imread("partofPR8.jpg");
	Mat src = imread("docexp2.jpg");

	if (src.empty())
	{
		cout << "get image error" << endl;
		return -1;
	}
	Mat gray;

	cvtColor(src, gray, CV_BGR2GRAY);

	//cvAdaptiveThreshold();
	imshow("Gray", gray);

	/*
	Mat gray;
	cvtColor(src, gray, CV_BGR2GRAY);
	/*
	for (int row = 0; row < src.rows; row++)
	{
		for (int col = 1; col < gray.cols; col++)
		{
			int value = getIntPixelValue(gray, row, col) - getIntPixelValue(gray, row, col - 1);
	//		dx.setPixel(row, col - 1, value);
			//cout << value << "  " << dx.getPixel(row, col - 1);
			//getchar();
		}
	}

	for (int col = 0; col < gray.cols; col++)
	{
		for (int row = 1; row < gray.rows; row++)
		{
			int value = getIntPixelValue(gray, row, col) - getIntPixelValue(gray, row - 1, col);
	//		dy.setPixel(row - 1, col, value);
		}
	}

	Mymat counter(gray.rows, gray.cols);
	Mymat cij(gray.rows, gray.cols);
	counter.create(gray);
	counter.window = 1;
	Mat canny = gray.clone();
	Mat result= gray.clone();

	Canny(gray, canny, 300, 100, 3);

	cout << getIntPixelValue(canny, 46, 171) << endl;
	//printf("%.2f", counter.getStd());

	///double alpha = (pow(counter.getStd()/128,0.95));
	double alpha = 0.1;
	for (int i = counter.window; i < cij.getrows() - counter.window; i++)
	{
		for (int j = counter.window; j < cij.getcols() - counter.window; j++)
		{
			//cij.setPixel(i, j, counter.getcijduc(i, j));
			cij.setPixel(i, j, (alpha*(counter.getcijduc(i, j) / counter.getcijadd(i, j)))
				+ ((1-alpha)*(counter.getcijduc(i,j))));
			//cout << cij.getPixel(i,j) << endl;
			//getchar();
		}
	}
	Mymat cannyMymat(gray.rows, gray.cols);
	cannyMymat.create(canny);
	cout << "width : " << counter.getEWfromCannyMat(canny) << endl;
	//usual use 2 EW as size of windows
	counter.window = 3 * counter.getEWfromCannyMat(canny);
	//Mat canny = gray.clone();
	counter.getResult(result);
	Mat test = gray.clone();
	cij.getMat(test);
	
	imshow("SRC", src);
	//imshow("cij", cij);
	imshow("initResult", result);

	//Mat comb = gray.clone();
	//combine(result,canny,comb);
	//imshow("combine",comb);

	Mat finalresult = gray.clone();
	initMat(finalresult);

	//removeSinglePixel(canny);
	removeSinglePixel(result);

	imshow("afterRemoveSinglePixel", result);

	//connect(canny, finalresult);
	connect(result, finalresult);
	removeSinglePixel(finalresult);

	imshow("finalResult", finalresult);
	*/

	//Canny
	Mat canny;
	Canny(gray, canny, 150, 60, 3);
	imshow("Canny", canny);

	Mymat grayM(gray);

	//gradient and contrast
	Mymat fig2aM(gray);
	//////////////////////////////////////
	//= 1;
	grayM.window = grayM.getEWfromCannyMat(canny);
	//////////////////////////////////////
	
	
	Mymat fig2bM(gray.rows, gray.cols);
	Mymat Imax(gray.rows, gray.cols);
	Mymat Imin(gray.rows, gray.cols);

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
			fig2aM.setPixel(row, col, max - min);

			fig2bM.setPixel(row, col, 255*(max - min)
				/ (max + min + 0.01));
			//cout << fig2bM.getPixel(row, col) << endl;
			//*/
		}
	}
	Mat fig2aMshow = gray.clone();
	fig2aM.getMat(fig2aMshow);
	imshow("fig2a gradient", fig2aMshow);


	Mat fig2bMshow = gray.clone();
	fig2bM.getMat(fig2bMshow);
	imshow("contrast", fig2bMshow);

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
	imshow("Calpha", Calphashow);

	//Otsu's
	Mat otsu;

	threshold(Calphashow, otsu, 0, 255, THRESH_OTSU);
	//reserve(otsu);
	imshow("otsu", otsu);

	//Combine
	Mat comb = gray.clone();
	myCombine(canny, otsu, comb);
	//removeSinglePixel(comb);
	//Mat connectRes = gray.clone();
	//initMat(connectRes);
	//connect(comb, connectRes);
	imshow("combine", comb);	
	
	//Ger result
	Mymat res(comb);
	//res.init(comb);
	//res.create(otsu);
	//grayM.window = grayM.getEWfromCannyMat(comb);
	//equal
	grayM.window = 1;

	Mat resshow = gray.clone();
	res.getMat(resshow);
	
	for (int i = grayM.window; i < grayM.getrows()- grayM.window; i++)
	{
		for (int j = grayM.window; j < grayM.getcols()- grayM.window; j++)
		{
			/*
			cout << (grayM.getStdAverage(i, j) / 2) << endl;
			cout << grayM.getPixel(i, j) << endl;
			cout << grayM.getMeanAverage(i, j) << endl;
			getchar();
			*/
			/*
			if (grayM.getPixel(i, j) <= (grayM.getMeanAverage(i, j)+ (grayM.getStdAverage(i, j) / 2)))
			{
				res.setPixel(i, j, 0);
			}
			else
			{
				res.setPixel(i, j, 210);
			}
			*/
			if (grayM.getPixel(i, j) <= (res.getMeanAverage(i, j) + (res.getStdAverage(i, j) / 2)))
			{
				//res.setPixel(i, j, 0);
				setPixelValue(resshow, i, j, 255);
			}
			else
			{
				//res.setPixel(i, j, 210);
				setPixelValue(resshow, i, j, 0);
			}
		}
	}
	
	//cout << "ave:"<< grayM.getMeanAverage(5, 5) + grayM.getStdAverage(5, 5) / 2 << endl;
	//cout << "ave:" << grayM.getMeanAverage(5, 5) << endl;
	/*
	Mat test = gray.clone();
	grayM.getMat(test);
	imshow("test", test);
	*/
	
	//removeSinglePixel(resshow);
	Mat resshow2 = gray.clone();
	connect(resshow, resshow2);
	removeSinglePixel(resshow2);
	imshow("resshow",resshow);
	imshow("resshow2", resshow2);

	waitKey();
	return 0;
}
