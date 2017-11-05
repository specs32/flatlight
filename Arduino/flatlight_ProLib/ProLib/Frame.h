#pragma once
#include "RGBColor.h"

class Frame {
public:
	Frame(RGBColor inputMatrix[8][6]);
	Frame(int inputMatrix[8][6], RGBColor* colors,int colorsSize);

	void getFrameMatrix(RGBColor toBeFilledMatrix[8][6]);
	void getSnailedFrameMatrix(RGBColor toBeFilledMatrix[8][6]);
private:
	RGBColor frameMatrix[8][6];

};