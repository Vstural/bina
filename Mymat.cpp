#include "Mymat.h"
#include "iostream"
#include "cstdlib"
#include "cmath"
#include "algorithm"

Mymat::Mymat()
{
}

Mymat::Mymat(cv::Mat graymat)
{
	Mymat::cols = graymat.cols;
	Mymat::rows = graymat.rows;
	data = new double[rows*cols];
	memset(Mymat::data, 0, sizeof(int));
	init(graymat);
	EW = 0;
}

Mymat::Mymat(int row ,int col)
{
	data = new double[row*col];
	Mymat::cols = col;
	Mymat::rows = row;
	memset(Mymat::data, 0, sizeof(int));
	EW = 0;
	return;
}

Mymat::~Mymat()
{
}

bool Mymat::init(cv::Mat graymat)
{
	uchar* p = graymat.data;
	//	mat->data[row*mat->cols + col] = value;
	for (int row = 0; row < rows; row++)
	{
		for (int col = 0; col < cols; col++)
		{
			//p[row*cols + col] = data[row*cols + col];
			data[row*cols + col] = p[row*cols + col];
		}
	}

	return true;
}

/*set gray mat*/
bool Mymat::getMat(cv::Mat &M)
{
	uchar *p = M.data;
	for (int row = 0; row < rows; row++)
	{
		for (int col = 0; col < cols; col++)
		{
			if (getPixel(row, col) <= 255)
			{
				if (getPixel(row, col) < 0)
				{
					p[row*cols + col] = 0;
				}
				else
				{
					p[row*cols + col] = getPixel(row, col);
				}
			}
			else
			{
				p[row*cols + col] = 255;
			}
			/*
			if (data[row*cols + col] - 150 > 0)
			{
				//p[row*cols + col] = data[row*cols + col] - 150;
				p[row*cols + col] = getPixel(row,col);
			}
			else
				p[row*cols + col] = 0;*/
		}
	}
	return true;
}

double Mymat::getPixel(int row, int col)
{
	return data[row*cols + col];
}
bool Mymat::setPixel(int row, int col,double value)
{
	data[row*cols + col] = value;
	return true;
}
int Mymat::getcols()
{
	return cols;
}
int Mymat::getrows()
{
	return rows;
}

/*
// out of range is unresolve,return 0 is possible
double Mymat::getcijadd(int row, int col)
{	
	return getintensityMax(row, col) + getintensityMin(row, col);
}
double Mymat::getcijduc(int row, int col)
{
	return getintensityMax(row, col) - getintensityMin(row, col);
}
*/

double Mymat::getintensityMax(int row, int col)
{
	int max = 0;
	for (int i = row - window; i < row + window; i++)
	{
		for (int j = col - window; j < col + window; j++)
		{
			if (getPixel(i, j) > max)
			{
				max = getPixel(i, j);
			}
		}
	}
	return max;
}

double Mymat::getintensityMin(int row, int col)
{
	int min = 255;
	for (int i = row - window; i < row + window; i++)
	{
		for (int j = col - window; j < col + window; j++)
		{
			if (getPixel(i, j) < min)
			{
				min = getPixel(i, j);
			}
		}
	}
	return min;
}


/*
double Mymat::getintensityMinMax(int row, int col,Mymat &matMin,Mymat matMax)
{
	int min = 255;
	int max = 0;
	for (int i = row - window; i < row + window; i++)
	{
		for (int j = col - window; j < col + window; j++)
		{
			if (getPixel(i, j) < min)
			{
				min = getPixel(i, j);
			}
			if (getPixel(i, j) > max)
			{
				max = getPixel(i, j);
			}
		}
	}
	matMax.setPixel(row, col, max);
	matMin.setPixel(row, col, min);
	return 0;
}
*/
double Mymat::getMeanAverage(int row, int col)
{
	double average = 0;
	for (int i = row - window; i <= row + window; i++)
	{
		for (int j = col - window; j <= col + window; j++)
		{
			/*
			if (getPixel(i, j) < min)
			{
				min = getPixel(i, j);
			}
			*/
			average += getPixel(i, j);
			//std::cout << "aaaa:" << average << std::endl;
		}
	}
	average /= pow(window*2+1,2);
	
	double sum = 0;
	for (int i = row - window; i <= row + window; i++)
	{
		for (int j = col - window; j <= col + window; j++)
		{
			sum += getPixel(i, j) - average;
		}
	}

	//sum /= (window*window);
	sum /= pow(window * 2 + 1, 2);

	return average;
}

double Mymat::getStdAverage(int row, int col)
{
	double average = 0;
	for (int i = row - window; i < row + window; i++)
	{
		for (int j = col - window; j < col + window; j++)
		{
			/*
			if (getPixel(i, j) < min)
			{
			min = getPixel(i, j);
			}
			*/
			average += getPixel(i, j);
		}
	}
	average /= pow(window * 2 + 1, 2);
	pow(window, 2);
	double sum = 0;
	for (int i = row - window; i < row + window; i++)
	{
		for (int j = col - window; j < col + window; j++)
		{
			sum += pow((getPixel(i, j) - average), 2);
		}
	}
	sum /= pow(window * 2 + 1, 2);
	sum = sqrt(sum);
	return sum;
}

double Mymat::getStd()
{
	double average = 0;
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			average += (getPixel(i, j) / (rows*cols));
		}
	}
	//average /= (window*window);
	pow(window, 2);
	double sum = 0;

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			sum += (pow((getPixel(i, j) - average), 2) / (rows*cols));
		}
	}
	//sum /= pow(window, 2);
	sum = sqrt(sum);
	return sum;
}

int Mymat::getIntPixelValue(cv::Mat mat, int row, int col)
{
	return mat.data[row*mat.cols + col];
}

void Mymat::setIntPixelValue(cv::Mat &mat, int row, int col, int value)
{
	mat.data[row*mat.cols + col] = value;
	//cout << value << "  " << int(mat.data[row*mat.cols + col]) << endl;
}

int Mymat::getEWfromCannyMat(cv::Mat mat)
{
	for (int row = 0; row < rows; row++)
	{
		for (int col = 0; col < cols; col++)
		{
			if (getIntPixelValue(mat, row, col) == 0 && getIntPixelValue(mat, row, col + 1) == 255)
			{
				if (getPixel(row,col) < getPixel(row,col+1))
				{
					setPixel(row, col, 0);
				}
			}
		}
	}

	std::vector<int> temp;
	for (int row = 0; row < rows; row++)
	{
		int x = 0;
		//searching for first 255
		/*
		while (getIntPixelValue(mat, row, x) != 255)
		{
			x++;
		}*/
		for (; x < cols; x++)
		{
			if (getIntPixelValue(mat, row, x) == 255)
			{
				break;
			}
		}

		for (int col = x; col < cols; col++)
		{
			if (getIntPixelValue(mat, row, col) == 255)
			{
				temp.push_back(col - x);
				x = col;
			}
		}
	}
	std::sort(temp.begin(),temp.end());
	//int EW;
	std::cout << temp.size() << std::endl;
	/*if (temp.size() % 2 == 0)
	{
		EW = temp[temp.size() / 2 + 1];
	}
	else
	{
		EW = temp[temp.size()];
	}
	//return EW;*/
	EW = temp[temp.size() / 2];
	return EW;
}

void Mymat::getResult(cv::Mat &mat)
{
	for (int i = window; i < rows - window; i++)
	{
		for (int j = window; j < cols - window; j++)
		{
			if (getMeanAverage(i,j)+getStdAverage(i,j)/2 >= getPixel(i,j))
			{
				setIntPixelValue(mat, i, j, 255);
			}
			else
			{
				setIntPixelValue(mat, i, j, 0);
			}
		}
	}
}
