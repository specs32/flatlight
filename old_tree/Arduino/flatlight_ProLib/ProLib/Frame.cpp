#include "Frame.h"
#include <stdlib.h>

Frame::Frame()
{
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			frameMatrix[i][j] = RGBColor(0,0,0);
		}
	}
}

Frame::Frame(RGBColor inputMatrix[8][6]) {
	setFrameMatrix(inputMatrix);
}

Frame::Frame(uint8_t inputMatrix[8][6], RGBColor* colors, uint8_t colorsSize)
{
	setFrameMatrix(inputMatrix, colors, colorsSize);
}

void Frame::getFrameMatrix(RGBColor toBeFilledMatrix[8][6])
{
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			toBeFilledMatrix[i][j] = frameMatrix[7-i][5-j];
		}
	}
}


void Frame::getSnailedFrameMatrix(RGBColor toBeFilledMatrix[8][6])
{
	for (int i = 0; i < 8; i++)
	{
		if(i%2 == 0)
		{
			for (int j = 0; j < 6; j++)
			{
				toBeFilledMatrix[i][j] = frameMatrix[7-i][5-j];
			}
		}
		else
		{
			for (int j = 0; j < 6; j++)
			{
				toBeFilledMatrix[i][j] = frameMatrix[7-i][j];
			}
		}
	}
}

void Frame::setFrameMatrix(RGBColor inputMatrix[8][6]) {
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			frameMatrix[i][j] = inputMatrix[i][j];
		}
	}
}

void Frame::setFrameMatrix(uint8_t inputMatrix[8][6], RGBColor* colors, uint8_t colorsSize)
{
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			int colorCode = inputMatrix[i][j];
			if (colorCode > colorsSize - 1)
			{
				colorCode = 0;
			}
			frameMatrix[i][j] = colors[colorCode];
		}
	}
}