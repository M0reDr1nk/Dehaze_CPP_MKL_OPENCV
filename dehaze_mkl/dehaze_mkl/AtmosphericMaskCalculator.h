#pragma once
#include "ImageMat.h"
#include "GuidedFilter.h"
#include "MKLConverter.h"


#define DEFAULT_NUM_OF_BINS 2000
class AtmosphericMaskCalculator
{
private:
	float r = 0;
	float eps = 0;
	float w = 0;
	float maxV = 0;
	int bins = DEFAULT_NUM_OF_BINS;
	GuidedFilter guidedFilter;
public:
	AtmosphericMaskCalculator(float r,float eps,float w,float maxV);
	struct AtmosphericMask calculate(const ImageMat & imageMat);
	~AtmosphericMaskCalculator();
};

struct AtmosphericMask{
	ImageMat maskMat;
	double A;
};