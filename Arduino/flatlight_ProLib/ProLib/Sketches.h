#pragma once
enum Shapes
{
	Hearth,
	Smiley
};

class Sketches
{
public:
	static void getSketch(int toBeFilledMatrix[8][6], Shapes shape);
};