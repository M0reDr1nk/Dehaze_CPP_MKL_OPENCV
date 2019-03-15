#pragma once
#include <opencv2/core/core.hpp>
#include <opencv.hpp>
#include <string>

class ImageRWManager
{
public:
	ImageRWManager();
	~ImageRWManager();
	cv::Mat read(std::string filePath);
	void write(cv::Mat image, std::string filePath);
};

