#include "pch.h"
#include "GuidedFilter.h"

GuidedFilter::GuidedFilter( float r, float eps)
{
	this->r = r;
	this->eps = eps;
}

GuidedFilter::GuidedFilter()
{
}
GuidedFilter::~GuidedFilter()
{
}

ImageMat GuidedFilter::apply(cv::Mat& cvMat) {
	
	cv::Mat tempMat0 = cv::Mat(cvMat.rows, cvMat.cols, CV_64FC1); 
	cv::Mat tempMat1 = cv::Mat(cvMat.rows, cvMat.cols, CV_64FC1);
	cv::Mat tempMat2 = cv::Mat(cvMat.rows, cvMat.cols, CV_64FC1);
	cv::Mat tempMat3 = cv::Mat(cvMat.rows, cvMat.cols, CV_64FC1);
	cv::Mat tempMat4 = cv::Mat(cvMat.rows, cvMat.cols, CV_64FC1);
	cv::Mat tempMat5 = cv::Mat(cvMat.rows, cvMat.cols, CV_64FC1);
	
	
	cv::boxFilter(cvMat, tempMat0, -1, cv::Size(this->r, this->r));//M_I
	cv::Mat cvMatZeroMeansMinFiltered=GuidedFilter::ZeroMeanMinFilter::apply(cvMat, 7);
	cv::boxFilter(cvMatZeroMeansMinFiltered, tempMat1, -1, cv::Size(this->r, this->r));//M_P
	ImageMat imageMatOrigin = MKLConverter::fromCVMat(cvMat);
	ImageMat imageMatZeroMeansMinFiltered = MKLConverter::fromCVMat(cvMatZeroMeansMinFiltered);
	ImageMat imageMatTemp0 = MKLConverter::fromCVMat(tempMat0);
	ImageMat imageMatTemp1 = MKLConverter::fromCVMat(tempMat1);

	//ImageMat convertMat(imageMatOrigin);
	//for (int i = 0; i < convertMat.cols*convertMat.rows;i++) {
	//	convertMat.matData[0][i] = 256.0;
	
	//}
	cv::boxFilter(MKLConverter::toCVMat(imageMatOrigin * imageMatZeroMeansMinFiltered), tempMat2, -1, cv::Size(this->r, this->r));//M_IP
	ImageMat imageMatTemp2 = MKLConverter::fromCVMat(tempMat2);
	ImageMat imageMatTemp3 = imageMatTemp2 - imageMatTemp0 * imageMatTemp1;//cov_Ip = m_Ip-m_I*m_p
	cv::boxFilter(MKLConverter::toCVMat(imageMatOrigin * imageMatOrigin), tempMat3, -1, cv::Size(this->r, this->r));//M_II
	ImageMat imageMatTemp4 = MKLConverter::fromCVMat(tempMat3);
	ImageMat imageMatTemp5 = imageMatTemp4 - imageMatTemp0* imageMatTemp0;//var_I = m_II-m_I*m_I

	ImageMat epsMat = imageMatOrigin;
	for (int i = 0; i < imageMatOrigin.cols*imageMatOrigin.rows; i++) {
		epsMat.matData[0][i] = this->eps;//TODO:CHECK THIS!!!

	} //MKLConverter::fromCVMat(cv::Mat(imageMatOrigin.rows, imageMatOrigin.cols, CV_64FC1, cv::Scalar(this->eps)));
	ImageMat test = imageMatTemp5 + epsMat;
	ImageMat imageMatTemp6 = imageMatTemp3 / (imageMatTemp5 + epsMat);   //a = cov_Ip / (var_I + eps)
	ImageMat imageMatTemp7 = imageMatTemp1 - imageMatTemp6 * imageMatTemp0;//b = m_p - a * m_I
	cv::boxFilter(MKLConverter::toCVMat(imageMatTemp6), tempMat4, -1, cv::Size(this->r, this->r));//m_a = cv2.boxFilter(a, -1, (r, r))
	cv::boxFilter(MKLConverter::toCVMat(imageMatTemp7), tempMat5, -1, cv::Size(this->r, this->r));//m_b = cv2.boxFilter(b, -1, (r, r))

	ImageMat resultMat= MKLConverter::fromCVMat(tempMat4)*imageMatOrigin + MKLConverter::fromCVMat(tempMat5);//m_a*I + m_b

	return resultMat;
}

cv::Mat GuidedFilter::ZeroMeanMinFilter::apply(const cv::Mat& cvMat, int r ) {
	cv::Mat cvMatFiltered;
	cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(2*r+1, 2*r+1), cv::Point(-1.0, -1.0));
	cv::erode(cvMat, cvMatFiltered, kernel);
	return cvMatFiltered;
}