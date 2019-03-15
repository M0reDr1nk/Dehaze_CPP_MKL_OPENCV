#include "pch.h"
#include "ImageRWManager.h"


ImageRWManager::ImageRWManager()
{
}


ImageRWManager::~ImageRWManager()
{
}

cv::Mat ImageRWManager::read(std::string const filePath)
{
	return cv::imread("demo.jpg",cv::IMREAD_ANYCOLOR);
}

void ImageRWManager::write(cv::Mat image,std::string filePath) 
{
	int postfix_pos = filePath.find(".jpg");
	filePath = filePath.substr(0, postfix_pos) + "_dehazed.jpg";
	cv::imwrite(filePath,image);
}