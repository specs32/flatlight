#pragma once

//#if ARDUINO >= 100
#include "Arduino.h"
//#else
//toobad
//#endif
#include "TemperatureManager.h"
#include "ButtonManager.h"
#include "WS2812Matrix.h"

class ProLibMain
{
public:
	ProLibMain();
public:
	//static ProLibMain& getInstance();
	void proLibMainLoop();
	void proTemperature();
	bool proButtons();
private:
	WS2812Matrix ledMatrix;
	Frame curFrame;
	boolean frameChanged = false;
	static TemperatureManager tempMng;
	ButtonManager buttonMng;
};
