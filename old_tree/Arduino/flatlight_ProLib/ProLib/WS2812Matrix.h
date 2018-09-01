#pragma once
#include "Light.h"
#include "WS2812.h"
#include "Frame.h"

class WS2812Matrix : public Light
{
public:
	// fades Relative to own Value e.g. 0.50 halves the light power
	void fadeRel(float value) override;
	// fades to Absolute value  e.g. 0.50 fades to halve the maximum light power
	void fadeAbs(float value) override;

	WS2812Matrix(int ledCount, int outputPin);

	// Sets the pixels to the frame
	void setFrame(Frame* frame);
private:
	int ledCount;
	WS2812 pixel;
};
