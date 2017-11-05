#pragma once

class RGBColor {
public:
	RGBColor(int r, int g, int b);
	RGBColor();
	//RGBColor(const RGBColor& other);
	int getR();
	int getG();
	int getB();
private:
	int r, g, b;


};