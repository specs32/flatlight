#include "Sketches.h"

void Sketches::getSketch(uint8_t toBeFilledMatrix[8][6], Shapes shape)
{
	if (shape == Filled)
	{
		uint8_t shapeMatrix[8][6] = {
			{ 0,0,0,0,0,0 },
			{ 0,0,0,0,0,0 },
			{ 0,0,0,0,0,0 },
			{ 0,0,0,0,0,0 },
			{ 0,0,0,0,0,0 },
			{ 0,0,0,0,0,0 },
			{ 0,0,0,0,0,0 },
			{ 0,0,0,0,0,0 }
		};
		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 6; j++)
			{
				toBeFilledMatrix[i][j] = shapeMatrix[i][j];
			}
		}
	}
	else if(shape == Hearth)
	{
		uint8_t shapeMatrix[8][6] = {
			{0,0,0,1,1,0},
			{0,0,1,1,1,1},
			{0,1,1,1,1,1},
			{1,1,1,1,1,0},
			{0,1,1,1,1,1},
			{0,0,1,1,1,1},
			{0,0,0,1,1,0},
			{0,0,0,0,0,0}
		};
		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 6; j++)
			{
				toBeFilledMatrix[i][j] = shapeMatrix[i][j];
			}
		}
		
	}
	else if (shape == Smiley)
	{
		uint8_t shapeMatrix[8][6] = {
			{ 0,1,1,1,1,0 },
			{ 1,1,1,1,1,1 },
			{ 1,2,1,1,2,1 },
			{ 1,1,1,1,1,1 },
			{ 1,2,2,2,2,1 },
			{ 1,1,1,3,3,1 },
			{ 1,1,1,1,1,1 },
			{ 0,1,1,1,1,0 }
		};
		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 6; j++)
			{
				toBeFilledMatrix[i][j] = shapeMatrix[i][j];
			}
		}
	}
}

