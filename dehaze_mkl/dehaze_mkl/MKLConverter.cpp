#include "pch.h"
#include "MKLConverter.h"


MKLConverter::MKLConverter()
{
}


MKLConverter::~MKLConverter()
{
}

ImageMat MKLConverter::fromCVMat(cv::Mat cvMatrix) {
	std::vector<double*> convertedMat;
	
    //double* channel_r = (double*)mkl_malloc(cvMatrix.rows*cvMatrix.cols * sizeof(double), 64);
	//double* channel_g = (double*)mkl_malloc(cvMatrix.rows*cvMatrix.cols * sizeof(double), 64);
	//double* channel_b = (double*)mkl_malloc(cvMatrix.rows*cvMatrix.cols * sizeof(double), 64);
	//convertedMat.push_back(channel_r);
	//convertedMat.push_back(channel_g);
	//convertedMat.push_back(channel_b);
	for (int i = 0; i <= cvMatrix.channels() - 1; i++) {
		convertedMat.push_back((double*)mkl_malloc(cvMatrix.rows*cvMatrix.cols * sizeof(double), 64));
	}
	cv::Mat splitedMatrix[3];
	cv::split(cvMatrix, splitedMatrix);

	//uchar *img_src_data = (uchar*)img_src->imageData;
	//std::cout << cvMatrix.channels();
	for (int i = 0; i <= cvMatrix.channels()-1; i++) {
	
		for (int j = 0; j < splitedMatrix[i].rows; j++)
		{
			for (int k = 0; k < splitedMatrix[i].cols; k++)
				convertedMat[i][j*splitedMatrix[i].cols + k] = (double)splitedMatrix[i].data[j*splitedMatrix[i].step[0] + k];
		}
	}


	return ImageMat(cvMatrix.rows, cvMatrix.cols, cvMatrix.channels()-1, convertedMat);
}

cv::Mat MKLConverter::toCVMat(const ImageMat& imageMat) {
	
	//uchar* cvMat = (uchar*)mkl_malloc(imageMat.rows*imageMat.cols*imageMat.dims , sizeof(uchar));

	cv::Mat cvMat;
	if (imageMat.dims == 0)
		cvMat = cv::Mat(imageMat.rows, imageMat.cols, CV_8UC1);
	if (imageMat.dims == 1)
		cvMat = cv::Mat(imageMat.rows, imageMat.cols, CV_8UC2);
	if (imageMat.dims==2)
		cvMat= cv::Mat(imageMat.rows, imageMat.cols, CV_8UC3);
	
	std::vector <unsigned char*>ucharMat;
	int size = imageMat.rows*imageMat.cols * sizeof(uchar);
	for (int i = 0; i <= imageMat.dims; i++) {
		ucharMat.push_back((uchar*)mkl_malloc(size, 64));
		memcpy(ucharMat[i], imageMat.matData[i], size);
		//memcpy(ucharMat[i], size, imageMat.matData[i], size);
	}


	for (int i = 0; i <= imageMat.dims; i++) {
		for (int j = 0; j < imageMat.rows; j++){
			for (int k = 0; k < imageMat.cols; k++)
				ucharMat[i][j*imageMat.cols + k] = (uchar)imageMat.matData[i][j*imageMat.cols + k];
		}
	}
	
	if (imageMat.dims == 0) {
		cvMat.data = ucharMat[0];
	}
	else {

		std::vector<cv::Mat> channelsMat;
		for (int i = 0; i <= imageMat.dims; i++) {
			channelsMat.push_back(cv::Mat(imageMat.rows, imageMat.cols, CV_8UC1));
			channelsMat[i].data = ucharMat[i];
		}
		cv::merge(channelsMat, cvMat);
	}
	return cvMat;
}