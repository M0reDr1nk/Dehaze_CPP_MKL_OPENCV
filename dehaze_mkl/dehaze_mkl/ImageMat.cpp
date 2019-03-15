#include "pch.h"
#include "ImageMat.h"


ImageMat::ImageMat(int rows, int cols, int dims, std::vector <double*> matData)
{
	this->rows = rows;
	this->cols = cols;
	this->dims = dims;
	this->matData = matData;
}
ImageMat::ImageMat()
{
	this->rows = 0;
	this->cols = 0;
	this->dims = 0;
	
}

ImageMat::~ImageMat()
{	std::vector<double *>::iterator i;
for ( i= this->matData.begin(); i != this->matData.end();i++) {
		;
	}
}

ImageMat::ImageMat(const ImageMat& imageMat) {
	this->rows = imageMat.rows;
	this->cols = imageMat.cols;
	this->dims = imageMat.dims;
	for (int i = 0; i <= this->dims; i++) {
		int size = imageMat.rows*imageMat.cols * sizeof(double);
		this->matData.push_back((double*)mkl_malloc(size, 64));
		memcpy_s(this->matData[i], size, imageMat.matData[i], size);
	}
	//this->matData.push_back();

}

ImageMat ImageMat::operator*(const ImageMat& imageMat) {
	if (imageMat.dims != this->dims) {
		throw("ERROR:Dimension of ImageMat must be equal!!");
	}
	ImageMat resultMat(imageMat);
	for (int i = 0; i <= this->dims; i++) {
		vdMul(this->cols*this->rows, this->matData[i], imageMat.matData[i], resultMat.matData[i]);
		//cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, this->rows, imageMat.cols, this->cols,
		//	1, this->matData[i], this->cols, imageMat.matData[i], imageMat.cols, 1, resultMat.matData[i], 0);
	}
	return resultMat;

}

ImageMat ImageMat::operator-(const ImageMat& imageMat) {
	if (imageMat.dims != this->dims) {
		throw("ERROR:Dimension of ImageMat must be equal!!");
	}
	ImageMat resultMat = ImageMat(imageMat);
	for (int i = 0; i <= this->dims; i++) {
		vdSub(this->cols*this->rows, this->matData[i], imageMat.matData[i], resultMat.matData[i]);
	}
	return resultMat;

}

ImageMat ImageMat::operator/(const ImageMat& imageMat) {
	if (imageMat.dims != this->dims) {
		throw("ERROR:Dimension of ImageMat must be equal!!");
	}
	ImageMat resultMat (imageMat);
	for (int i = 0; i <= this->dims; i++) {
		vdDiv(this->cols*this->rows, this->matData[i], imageMat.matData[i], resultMat.matData[i]);
	}
	return resultMat;

}
ImageMat ImageMat::operator+(const ImageMat& imageMat) {
	if (imageMat.dims != this->dims) {
		throw("ERROR:Dimension of ImageMat must be equal!!");
	}
	ImageMat resultMat = ImageMat(imageMat);
	for (int i = 0; i <= this->dims; i++) {
		vdAdd(this->cols*this->rows, this->matData[i], imageMat.matData[i], resultMat.matData[i]);
	}
	return resultMat;

}
//UcharImageMat::UcharImageMat(int rows, int cols, int dims, std::vector <uchar*> matData){
//	this->rows = rows;
//	this->cols = cols;
//	this->dims = dims;
//	this->matData = matData;
//}


//UcharImageMat::UcharImageMat(const UcharImageMat& imageMat) {
//	this->rows = imageMat.rows;
//	this->cols = imageMat.cols;
//	this->dims = imageMat.dims;
//	for (int i = 0; i <= this->dims; i++) {
//		int size = imageMat.rows*imageMat.cols * sizeof(uchar);
//		this->matData.push_back((uchar*)mkl_malloc(size, 64));
//		memcpy_s(this->matData[i], size, imageMat.matData[i], size);
//	}
//
//}
