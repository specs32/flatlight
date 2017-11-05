#include "Sketches.h"

void Sketches::getSketch(int toBeFilledMatrix[8][6], Shapes shape)
{
	if(shape == Hearth)
	{
		int toBeFilledMatrix2[8][6] = {
			{0,0,0,1,1,0},
			{0,0,1,1,1,1},
			{0,1,1,1,1,1},
			{1,1,1,1,1,0},
			{0,1,1,1,1,1},
			{0,0,1,1,1,1},
			{0,0,0,1,1,0},
			{0,0,0,0,0,0}
		};
	}
	if (shape == Smiley)
	{
		int toBeFilledMatrix2[8][6] = {
			{ 0,1,1,1,1,0 },
			{ 1,1,1,1,1,1 },
			{ 1,2,1,1,2,1 },
			{ 1,1,1,1,1,1 },
			{ 1,2,2,2,2,1 },
			{ 1,1,1,3,3,1 },
			{ 1,1,1,1,1,1 },
			{ 0,1,1,1,1,0 }
		};
	}
}

