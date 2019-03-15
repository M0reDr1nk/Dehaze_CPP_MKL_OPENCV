#include "pch.h"
#include "AtmosphericMaskCalculator.h"
//#include <iostream>
//#include <math.h>
#include <algorithm>



AtmosphericMaskCalculator::AtmosphericMaskCalculator(float r, float eps, float w, float maxV)
{
	this->r = r;
	this->eps = eps;
	this->w = w;
	this->maxV = maxV;
	this->guidedFilter = GuidedFilter::GuidedFilter(r, eps);
	
}
struct AtmosphericMask AtmosphericMaskCalculator::calculate(const ImageMat & imageMat) {
	
	cv::Mat darkChannelCvMat = cv::Mat(imageMat.rows, imageMat.cols, CV_64FC1);
	ImageMat darkChannelMat = MKLConverter::fromCVMat(darkChannelCvMat);
	for (int row = 0; row < imageMat.rows; row++) {
		for (int col = 0; col < imageMat.cols; col++) {
			double temp = fmin(imageMat.matData[0][row*imageMat.cols + col], imageMat.matData[1][row*imageMat.cols + col]);
			double min = fmin(temp, imageMat.matData[2][row*imageMat.cols + col]);
			darkChannelMat.matData[0][row*imageMat.cols + col] = min;
		}
	}
	ImageMat convertMat(darkChannelMat);
	for (int i = 0; i <= convertMat.dims; i++) {
		for (int j = 0; j < convertMat.cols*convertMat.rows; j++) {
			convertMat.matData[i][j] = 256.0;
		}
	}

	cv::Mat  darkChannelFilteredcvMat=cv::Mat(imageMat.rows, imageMat.cols, CV_64FC1);
	ImageMat convertDarkChannelMat = darkChannelMat * convertMat;
	MKLConverter::toCVMat(convertDarkChannelMat).copyTo(darkChannelFilteredcvMat);
	ImageMat darkChannelFilteredMat = this->guidedFilter.apply(darkChannelFilteredcvMat)/convertMat/convertMat;
	double max_val=0, min_val=0;
	for (int i = 0; i < darkChannelFilteredMat.cols*darkChannelFilteredMat.rows; i++) {
		if (darkChannelFilteredMat.matData[0][i] > max_val) {
			max_val = darkChannelFilteredMat.matData[0][i];
		}
		if (darkChannelFilteredMat.matData[0][i] < min_val) {
			min_val = darkChannelFilteredMat.matData[0][i];
		}
	}
	std::vector<double> rangeOfHistogram;
	for (int i = 0; i <= this->bins; i++) {
		rangeOfHistogram.push_back(min_val + i * (max_val - min_val) / this->bins );
	}
	std::vector<int>countOfHistogram(this->bins,0);
	for (int i = 0; i < darkChannelFilteredMat.rows*darkChannelFilteredMat.cols; i++) {
		//std::cout<< ((darkChannelFilteredMat.matData[0][i]-min_val)/(max_val-min_val)*(this->bins-1)) <<std::endl;
		countOfHistogram[round((darkChannelFilteredMat.matData[0][i] - min_val) / (max_val - min_val)*(this->bins - 1))] += 1;
	}
	int count = 0;
	int levelMax = 0;
	for (int i = this->bins - 1; i >= 0; i--) {
		count += countOfHistogram[i];
		if (count >= std::round(darkChannelFilteredMat.cols*darkChannelFilteredMat.rows*(1 - 0.999))) {
			levelMax = i;
			break;
		}
	}
	ImageMat convertMat_1(imageMat);
	for (int i = 0; i <= imageMat.dims; i++) {
		for (int j = 0; j < convertMat_1.cols*convertMat_1.rows; j++) {
			convertMat_1.matData[i][j] = 256.0;
		}
	}
	cv::Mat splitedMatrix[3];
	ImageMat temp(convertMat_1);
	temp=temp* imageMat;
	cv::split(MKLConverter::toCVMat(temp), splitedMatrix);
	ImageMat channelBMat = MKLConverter::fromCVMat(splitedMatrix[0]);
	ImageMat channelGMat = MKLConverter::fromCVMat(splitedMatrix[1]);
	ImageMat channelRMat = MKLConverter::fromCVMat(splitedMatrix[2]);
	ImageMat meanMat = channelBMat + channelGMat + channelRMat;
	double max = 0;
	
	for (int i = 0; i < darkChannelFilteredMat.rows*darkChannelFilteredMat.cols; i++) {
		//std::cout << i;
		//std::cout << darkChannelFilteredMat.matData[0][i] * this->w<<std::endl;
		if (darkChannelFilteredMat.matData[0][i] >= rangeOfHistogram[levelMax]) {
			if (meanMat.matData[0][i]>max) {
				max = meanMat.matData[0][i];
			}
		}
		if ((darkChannelFilteredMat.matData[0][i] *this->w)>this->maxV ) {
			
			darkChannelFilteredMat.matData[0][i] = this->maxV;
		}
	}
	max /= 3.0*256;
	static AtmosphericMask atmosphericMask;
	atmosphericMask.maskMat = darkChannelFilteredMat;
	atmosphericMask.A = max;
	return atmosphericMask;
}

AtmosphericMaskCalculator::~AtmosphericMaskCalculator()
{
}
