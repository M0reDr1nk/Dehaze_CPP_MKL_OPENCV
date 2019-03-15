#include "pch.h"
#include <iostream>
#include "ImageRWManager.h"
#include "MKLConverter.h"
#include <string>
#include "ImageMat.h"
#include "AtmosphericMaskCalculator.h"

int main()
{
	std::string imageFilePath= "demo.jpg";
	//====INIT====
	float r = 81;
	float eps = 0.001; 
	float w = 0.95; 
	float maxV = 0.80;
	ImageRWManager imageRWManager=ImageRWManager();
	AtmosphericMaskCalculator atmosphericMaskCalculator = AtmosphericMaskCalculator(r,eps,w,maxV);
	//====END_INIT====
	cv::Mat image = imageRWManager.read(imageFilePath);
	ImageMat imageMat= MKLConverter::fromCVMat(image);
	ImageMat convertMat(imageMat);
	for (int i = 0; i <= convertMat.dims; i++) {
		for (int j = 0; j < convertMat.cols*convertMat.rows; j++) {
			convertMat.matData[i][j] = 256.0;
		}
	}
	imageMat = imageMat / convertMat;
	
	AtmosphericMask atmosphericMask=atmosphericMaskCalculator.calculate(imageMat);
	std::vector<cv::Mat>splitedcvMat;
	std::vector<ImageMat>splitedMat;
	//cv::Mat cvMat=MKLConverter::toCVMat(imageMat);
	cv::split(image, splitedcvMat);
	ImageMat aMat (atmosphericMask.maskMat);
	for (int i = 0; i < aMat.rows*aMat.cols; i++) {
		//std::cout<< ((darkChannelFilteredMat.matData[0][i]-min_val)/(max_val-min_val)*(this->bins-1)) <<std::endl;
		aMat.matData[0][i] = atmosphericMask.A;
	}
	ImageMat oneMat(atmosphericMask.maskMat);
	for (int i = 0; i < aMat.rows*aMat.cols; i++) {
		//std::cout<< ((darkChannelFilteredMat.matData[0][i]-min_val)/(max_val-min_val)*(this->bins-1)) <<std::endl;
		oneMat.matData[0][i] = 1.0;
	}
	ImageMat convertMat_1(atmosphericMask.maskMat);
	for (int i = 0; i <= convertMat_1.dims; i++) {
		for (int j = 0; j < convertMat_1.cols*convertMat_1.rows; j++) {
			convertMat_1.matData[i][j] = 256.0;
		}
	
	
	for (int i = 0; i <=imageMat.dims; i++) {
		//std::cout << *(MKLConverter::fromCVMat(splitedcvMat[i]) - atmosphericMask.maskMat).matData[0];
		//MKLConverter::fromCVMat(splitedcvMat.at(i));
		
		splitedMat.push_back((MKLConverter::fromCVMat(splitedcvMat[i]) -atmosphericMask.maskMat*convertMat_1) /
			(oneMat - atmosphericMask.maskMat / aMat));
		//splitedMat.push_back((MKLConverter::fromCVMat(splitedcvMat[i])));
	}

	}
	for (int i = 0; i <=imageMat.dims; i++) {
		//splitedMat[i] = splitedMat[i];
		splitedcvMat[i] = MKLConverter::toCVMat(splitedMat[i]);
	}
	
	cv::Mat resultCvMat=cv::Mat(splitedcvMat[0].rows,splitedcvMat[0].cols,CV_8UC3);
	cv::merge(splitedcvMat, resultCvMat);
	imageRWManager.write(resultCvMat,imageFilePath+"_mod");
	cv::imshow("TEST", resultCvMat);
	cv::waitKey(1);
	system("PAUSE");
	return 0;
}

