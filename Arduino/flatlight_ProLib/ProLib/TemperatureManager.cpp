#include "TemperatureManager.h"
#include <Arduino.h>

TemperatureManager::TemperatureManager()
{
	
}

TemperatureManager& TemperatureManager::getInstance()
{
	static TemperatureManager instance;
	return instance;
}

double TemperatureManager::checkTemperature()
{
	// Reads in the temp from InputPin0 and returns it in Celsius
	int val = analogRead(0);
	double temp;
	temp = log(10000.0 * ((1024.0 / val - 1)));
	temp = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * temp * temp)) * temp);
	temp = temp - 273.15;
	return temp;
}
