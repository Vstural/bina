#pragma once
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
class Mymat
{
public:
	Mymat();
	Mymat(cv::Mat graymat);
	Mymat(int row,int col);
	~Mymat();
	/*form 0 to n-1*/
	double getPixel(int row, int col);
	bool setPixel(int row, int col, double value);
	int getcols();
	int getrows();
	bool init(cv::Mat graymat);
	bool getMat(cv::Mat &M);
	int window = 1;

	/*
	double getcijadd(int row, int col);
	double getcijduc(int row, int col);

	/*define the windows of int getcij(int row,int col)

	double getintensityMax(int row, int col);
	double getintensityMin(int row, int col);*/
	/*get mean average according window*/
	double getMeanAverage(int row, int col);
	double getStdAverage(int row, int col);
	int getEWfromCannyMat(cv::Mat mat);
	void getResult(cv::Mat &mat);
	//double getintensityMinMax(int row, int col, Mymat &min, Mymat max);
	double getStd();

protected:
	int rows;
	int cols;
	double* data;
	int getIntPixelValue(cv::Mat mat, int row, int col);
	void setIntPixelValue(cv::Mat &mat, int row, int col, int value);
	int EW = 0;
};

