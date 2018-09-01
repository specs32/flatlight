#pragma once
class Light
{
public:
	// fades Relative to own Value e.g. 0.50 halves the light power
	virtual  void fadeRel(float value) = 0;
	// fades to Absolute value  e.g. 0.50 fades to halve the maximum light power
	virtual  void fadeAbs(float value) = 0;
};