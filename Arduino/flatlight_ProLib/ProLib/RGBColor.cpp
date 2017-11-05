#include "RGBColor.h"


RGBColor::RGBColor(int r, int g, int b) {
	this->r = r;
	this->g = g;
	this->b = b;
}
RGBColor::RGBColor() {
	r = 0;
	g = 0;
	b = 0;
}
int RGBColor::getR() {
	return r;
}
int RGBColor::getG() {
	return g;
}
int RGBColor::getB() {
	return b;
}
