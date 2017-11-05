#include "ButtonManager.h"
#include <Arduino.h>

/*#ifndef HOLD_TIME
#define HOLD_TIME 250
#endif*/


ButtonManager::ButtonManager()
{
	lastTime = millis();
}

/*ButtonManager& ButtonManager::getInstance()
{
	static ButtonManager instance = ButtonManager();
	return instance;
}*/

// Returns the ButtonStatus (NotPressed = 0, Pressed = 1, Hold = 2, Released =3) and the Time since the last Press/Release
void ButtonManager::readButtons(int* status, long* time)
{
	for (int i = 0; i < nrOfButtons; i++)
	{
		long curTime = millis();
		int reading = digitalRead(buttonPins[i]);
		time[i] = curTime - triggerTime[i];
		//button first pressed
		if (reading == HIGH && lastReading[i] == LOW) {
			triggerTime[i] = millis();
			status[i] = 1;
		}

		//button held 
		else if (reading == HIGH && lastReading[i] == HIGH) {
			status[i] = 2;
		}

		// button released

		else if (reading == LOW && lastReading[i] == HIGH) {
			triggerTime[i] = millis();
			status[i] = 3;
		}
		else // if it is not Pressed
		{
			status[i] = 0;
		}
		lastReading[i] = reading;
	}
}

// Returns the Button Nr if successfull otherwise -1
int ButtonManager::addButton(int buttonPin)
{
	if (nrOfButtons < MAX_BUTTONS)
	{
		buttonPins[nrOfButtons] = buttonPin;
		triggerTime[nrOfButtons] = millis();
		lastReading[nrOfButtons] = digitalRead(buttonPin);
		nrOfButtons++;
		return (nrOfButtons-1);
	}
	else { return -1; }
}

/*int ButtonManager::removeButton(int buttonNr)		// I dont think that we will need this
{
}*/
