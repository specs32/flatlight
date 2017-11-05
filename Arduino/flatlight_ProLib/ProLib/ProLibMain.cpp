#include "ProLibMain.h"
#include <stdlib.h>
#include <math.h>

// define PINS and other variables


/*int bounceTime = 50;              // define button debounce
int holdTime = 250;               // define button hold time
int doubleTime = 500;             // define button doubleclick time
int lastReading = LOW;            // button read value
int hold = 0;                     // button hold value
int single = 0;                   // button single press value
long onTime = 0;                  // button read ontime
long lastSwitchTime = 0;          // button switchtime value

int lightMode = 0;                // set lightmode value
int lightState = 0;                // set lightstate value

int xbounceTime = 50;              // define button debounce
int xholdTime = 250;               // define button hold time
int xdoubleTime = 500;             // define button doubleclick time
int xlastReading = LOW;            // button read value
int xhold = 0;                     // button hold value
int xsingle = 0;                   // button single press value
long xonTime = 0;                  // button read ontime
long xlastSwitchTime = 0;          // button switchtime value

int xlightMode = 1;                // set lightmode value
int XLEDstate = 0;               // on poweron start LED in lowest mode
int xpreviousLEDstate = 8;       // set previous ledstate 

long steps = 0;                  // value for smooth fading*/

#ifndef xbutton
#define xbutton 8          // define the other button pin ^^
#define button 9		        // define button pin
#define xled = 12;                // Cree (X)LEDs
#define RED 17               // Battery Indicator Pin
#define ORANGE 18            // Battery Indicator Pin
#define YELLOW 19            // Battery Indicator Pin
#define GREEN 20             // Battery Indicator Pin
#define BRIGHTGREEN 21       // Battery Indicator Pin
#define templed 22

#define LEDCount 48
#define outputPin 13

#define BOUNCE_TIME 50
#define HOLD_TIME 250

#endif


ProLibMain::ProLibMain() 
	: Pixel(WS2812(LEDCount)),
	buttonMng(ButtonManager())
{
	tempMng = TemperatureManager::getInstance();

	pinMode(button, INPUT);
	pinMode(xbutton, INPUT);
	pinMode(templed, OUTPUT);
	pinMode(RED, OUTPUT);
	pinMode(ORANGE, OUTPUT);
	pinMode(YELLOW, OUTPUT);
	pinMode(GREEN, OUTPUT);
	pinMode(BRIGHTGREEN, OUTPUT);
	Pixel.setOutput(outputPin);

	buttonMng.addButton(xbutton);
	buttonMng.addButton(button);

	Pixel.setColorOrderGRB(); // for sk6812 rgbw pixel

	// Setting the Pixel to 0,0,0;
	cRGB rgbValue;
	rgbValue.r = 00;
	rgbValue.g = 00;
	rgbValue.b = 00;
	for (int i = 0; i < LEDCount; i++)
	{
		Pixel.set_crgb_at(i, rgbValue);
	}
}

ProLibMain& ProLibMain::getInstance()
{
	static ProLibMain instance;
	return instance;
}

void ProLibMain::proLibMainLoop()
{
	static int c;
	c++;
	c %= 100;
	cRGB rgbValue;
	rgbValue.r = c;
	rgbValue.g = 0;
	rgbValue.b = 25;
	Pixel.set_crgb_at(1, rgbValue);

	Pixel.sync();
}

void ProLibMain::proTemperature()
{
	double temp = tempMng.checkTemperature();
	//proLib.proLibMainLoop();
	// if temp exceeds limit blink warning led and set LED output to lowest LEDstate, lets cool down a little
	// ntc directly coupled to CREE LED thermal pad, so 80 °C might be okay..ish ?!
	bool overheat = false;
	/*
	int val;
	double temp2;
	val=analogRead(0);
	temp2=Thermistor(val);

	if(temp-temp2<5 && temp2-temp < 5)  // For Debugging Purposes
	{
	digitalWrite(GREEN, 1);
	delay(500);
	digitalWrite(GREEN, 0);
	delay(500);
	}*/
	if (temp > 80) {
		overheat = true;
	}
	while (overheat) {
		digitalWrite(templed, 1);
		delay(50);
		digitalWrite(templed, 0);
		delay(50);


		//TODO: Clean Code
		// fade down XLED
		//XLEDstate = 8;      // so you dont walk in total darkness all of a sudden ;)
		//xpreviousLEDstate = XLEDstate;
		//fade();

		// blank Matrix
		//blank();

		// doublecheck
		temp = tempMng.checkTemperature();

		// if temperatur falls below 60°C go to some higher brightness ...

		if (temp < 60) {
			digitalWrite(YELLOW, 1);
			delay(500);
			digitalWrite(YELLOW, 0);
			delay(500);
			// give some brightness back
			//XLEDstate = 100;
			//xpreviousLEDstate = XLEDstate;
			//fade();

			overheat = false;

		}
	}
	
}

void ProLibMain::proButtons()
{

	//ButtonManager buttonMng2 = ButtonManager::getInstance();
	buttonMng.addButton(xbutton);
	buttonMng.addButton(button);
	
	int buttonStatus[2] = {0,0};
	long buttonTime[2] = { 0,0 };

	buttonMng.readButtons(buttonStatus,buttonTime);
	// Reads in the Button Status (NotPressed = 0, Pressed = 1, Hold = 2, Released =3) and the Time since the last Press/Release

	// Button 0 is xbutton 1 is button
	// if xButton is Pressed (and we are sure it is no bounce)
	/*if (buttonStatus[0] == 0)
	{
		digitalWrite(BRIGHTGREEN, 1);
	}
	else
	{
		digitalWrite(BRIGHTGREEN, 0);
	}
	if (buttonStatus[0] == 1)
	{
		digitalWrite(GREEN, 1);
		delay(500);
	}
	else
	{
		digitalWrite(GREEN, 0);
	}
	if (buttonStatus[0] == 2)
	{
		digitalWrite(YELLOW, 1);
	}
	else
	{
		digitalWrite(YELLOW, 0);
	}
	if (buttonStatus[0] == 3)
	{
		digitalWrite(ORANGE, 1);
		delay(500);
	}
	else
	{
		digitalWrite(ORANGE, 0);
	}
	if (buttonStatus[0] == 4)
	{
		digitalWrite(RED, 1);
	}
	else
	{
		digitalWrite(RED, 0);
	}*/
	if(buttonStatus[0] == 1 && buttonTime[0] > BOUNCE_TIME)
	{
		digitalWrite(GREEN, 1);
		
	}
	// when it is hold long enough
	else if (buttonStatus[0] == 2 && buttonTime[0] > HOLD_TIME)
	{
		digitalWrite(YELLOW, 1);
		
	}
	// when it is released after Holding
	else if (buttonStatus[0] == 3 && buttonTime[0] > HOLD_TIME)
	{
		digitalWrite(ORANGE, 1);
		
	}
	// when it is released without Holding
	else if (buttonStatus[0] == 3 && buttonTime[0] < HOLD_TIME && buttonTime[0] > BOUNCE_TIME)
	{
		digitalWrite(RED, 1);
		
	}
	// The same for Button press
	if (buttonStatus[1] == 1 && buttonTime[1] > BOUNCE_TIME)
	{

	}
	// hold
	else if (buttonStatus[1] == 2 && buttonTime[1] > HOLD_TIME)
	{
		digitalWrite(RED, 0);
		digitalWrite(GREEN, 0);
		digitalWrite(YELLOW, 0);
		digitalWrite(ORANGE, 0);
	}
	// hold release
	else if (buttonStatus[1] == 2 && buttonTime[1] > HOLD_TIME)
	{

	}
	// not hold release
	else if (buttonStatus[1] == 2 && buttonTime[1] < HOLD_TIME && buttonTime[1] > BOUNCE_TIME)
	{

	}

}
