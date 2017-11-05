#pragma once

//#if ARDUINO >= 100
#include "Arduino.h"
//#else
//toobad
//#endif
#include "WS2812.h"
#include "TemperatureManager.h"
#include "ButtonManager.h"

#define LEDCount 48

class ProLibMain
{
public:
	ProLibMain();	// Singleton
public:
	static ProLibMain& getInstance();
	void proLibMainLoop();
	void proTemperature();
	void proButtons();
private:
	WS2812 Pixel;
	static TemperatureManager tempMng;
	ButtonManager buttonMng;
};
