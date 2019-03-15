#pragma once
#include<mkl.h>
#include<vector>
#include<opencv.hpp>
#include"ImageMat.h"

class MKLConverter
{
public:
	MKLConverter();
	~MKLConverter();
	static ImageMat  fromCVMat(cv::Mat cvMatrix);
	static cv::Mat toCVMat(const ImageMat& imageMat);
};

