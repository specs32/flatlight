#pragma once

class TemperatureManager
{
private:
	TemperatureManager();	// Singleton
public:
	static TemperatureManager& getInstance();
	double checkTemperature();
};