#pragma once
#include <vector>
#include<mkl.h>
#include<opencv.hpp>

class ImageMat
{
public:
	int rows = 0;
	int cols = 0;
	int dims = 0;
	std::vector <double*> matData;
	ImageMat();
	ImageMat(int rows,int cols,int dims, std::vector <double*> matData);
	ImageMat(const ImageMat& imageMat);
	ImageMat operator *(const ImageMat& imageMat);
	ImageMat operator -(const ImageMat& imageMat);
	ImageMat operator +(const ImageMat& imageMat);
	ImageMat operator /(const ImageMat& imageMat);
	~ImageMat();
};
