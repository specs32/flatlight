#pragma once
#include "RGBColor.h"

class Frame {
public:
	Frame();
	Frame(RGBColor inputMatrix[8][6]);
	Frame(uint8_t inputMatrix[8][6], RGBColor* colors, uint8_t colorsSize);

	void getFrameMatrix(RGBColor toBeFilledMatrix[8][6]);
	void getSnailedFrameMatrix(RGBColor toBeFilledMatrix[8][6]);

	void setFrameMatrix(RGBColor inputMatrix[8][6]);
	void setFrameMatrix(uint8_t inputMatrix[8][6], RGBColor* colors, uint8_t colorsSize);
private:
	RGBColor frameMatrix[8][6];

};