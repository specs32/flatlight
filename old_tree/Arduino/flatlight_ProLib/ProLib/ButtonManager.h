#pragma once
#ifndef MAX_BUTTONS
#define MAX_BUTTONS 2
#endif

class ButtonManager
{
public:
	ButtonManager();
	//static ButtonManager& getInstance();
	void readButtons(int* status, long* time);
	int addButton(int buttonPin);
	//int removeButton(int buttonNr);

private:
	int nrOfButtons = 0;
	int buttonPins[MAX_BUTTONS] = { 0,0 };
	long triggerTime[MAX_BUTTONS] = { 0,0 };
	int lastReading[MAX_BUTTONS] = { 0,0 };
	long lastTime;
};

