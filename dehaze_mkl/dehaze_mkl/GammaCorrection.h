#pragma once
#include <opencv.hpp>
class GammaCorrection
{
public:
	GammaCorrection();
	~GammaCorrection();
	cv::Mat apply(cv::Mat image);
};


