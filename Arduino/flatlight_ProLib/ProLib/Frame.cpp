#include "Frame.h"
#include <stdlib.h>

Frame::Frame(RGBColor inputMatrix[8][6]) {
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			frameMatrix[i][j] = inputMatrix[i][j];
		}
	}
}

Frame::Frame(int inputMatrix[8][6], RGBColor* colors, int colorsSize)
{
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			int colorCode = inputMatrix[i][j];
			if(colorCode > colorsSize)
			{
				colorCode = 0;
			}
			frameMatrix[i][j] = colors[colorCode];
		}
	}
}

void Frame::getFrameMatrix(RGBColor toBeFilledMatrix[8][6])
{
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			toBeFilledMatrix[i][j] = frameMatrix[i][j];
		}
	}
}


void Frame::getSnailedFrameMatrix(RGBColor toBeFilledMatrix[8][6])
{
	for (int i = 0; i < 8; i++)
	{
		if(i%2 == 0){
			for (int j = 0; j < 6; j++)
			{
				toBeFilledMatrix[i][j] = frameMatrix[i][j];
			}
		}
		else
		{
			for (int j = 0; j < 6; j++)
			{
				toBeFilledMatrix[i][5-j] = frameMatrix[i][j];
			}
		}
	}
}
