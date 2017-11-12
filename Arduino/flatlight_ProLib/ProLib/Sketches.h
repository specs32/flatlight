#pragma once
#include <stdint.h>

enum Shapes
{
	Filled,
	Hearth,
	Smiley
};

class Sketches
{
public:
	static void getSketch(uint8_t toBeFilledMatrix[8][6], Shapes shape);
};