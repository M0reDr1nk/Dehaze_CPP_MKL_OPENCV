#pragma once
#include <opencv.hpp>
#include "ImageMat.h"
#include "MKLConverter.h"

class GuidedFilter
{
private:
	
	float r = 0;
	float eps = 0;
	class ZeroMeanMinFilter {
	public:
		static cv::Mat apply(const cv::Mat& cvMat, int r = 7);
		
		
	};
public:
	GuidedFilter();
	GuidedFilter( float r, float eps);
	~GuidedFilter();
	ImageMat apply( cv::Mat& cvMat);
};

