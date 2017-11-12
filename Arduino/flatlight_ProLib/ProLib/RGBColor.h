#pragma once
#include <stdint.h>

class RGBColor {
public:
	RGBColor(uint8_t r, uint8_t g, uint8_t b);
	RGBColor();
	//RGBColor(const RGBColor& other);
	uint8_t getR();
	uint8_t getG();
	uint8_t getB();
private:
	uint8_t r, g, b;


};
