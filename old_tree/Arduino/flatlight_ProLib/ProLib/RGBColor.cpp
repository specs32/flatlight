#include "RGBColor.h"


RGBColor::RGBColor(uint8_t r, uint8_t g, uint8_t b) {
	this->r = r;
	this->g = g;
	this->b = b;
}
RGBColor::RGBColor() {
	r = 0;
	g = 0;
	b = 0;
}
uint8_t RGBColor::getR() {
	return r;
}
uint8_t RGBColor::getG() {
	return g;
}
uint8_t RGBColor::getB() {
	return b;
}
