#include "ProLibMain.h"
#include <stdlib.h>
#include <math.h>
#include "Sketches.h"

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

#ifndef BUTTON_SUN
#define BUTTON_SUN 8          // define the other button pin ^^
#define BUTTON_MOON 9		        // define button pin
#define xled = 12;                // Cree (X)LEDs
#define RED 17               // Battery Indicator Pin
#define ORANGE 18            // Battery Indicator Pin
#define YELLOW 19            // Battery Indicator Pin
#define GREEN 20             // Battery Indicator Pin
#define BRIGHTGREEN 21       // Battery Indicator Pin
#define templed 22

#define LED_COUNT 48
#define LED_MATRIX_OUTPUT_PIN 13
#define LED_LIGHT_MODES 6

#define BOUNCE_TIME 50
#define HOLD_TIME 250
#define DOUBLE_CLICK_TIME 500

#endif


ProLibMain::ProLibMain() 
	:	buttonMng(ButtonManager()),
	ledMatrix(LED_COUNT, LED_MATRIX_OUTPUT_PIN),
	curFrame()
{
	tempMng = TemperatureManager::getInstance();

	pinMode(BUTTON_MOON, INPUT);
	pinMode(BUTTON_SUN, INPUT);
	pinMode(templed, OUTPUT);
	pinMode(RED, OUTPUT);
	pinMode(ORANGE, OUTPUT);
	pinMode(YELLOW, OUTPUT);
	pinMode(GREEN, OUTPUT);
	pinMode(BRIGHTGREEN, OUTPUT);


	buttonMng.addButton(BUTTON_SUN);
	buttonMng.addButton(BUTTON_MOON);
}

/*ProLibMain& ProLibMain::getInstance()
{
	static ProLibMain instance;
	return instance;
}*/

void ProLibMain::proLibMainLoop()
{
	if(frameChanged)
	{
		ledMatrix.setFrame(&curFrame);
		frameChanged = false;
	}
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

bool ProLibMain::proButtons()
{
	bool batteryCheck = false;

	//ButtonManager buttonMng2 = ButtonManager::getInstance();
	buttonMng.addButton(BUTTON_SUN);
	buttonMng.addButton(BUTTON_MOON);
	
	int buttonStatus[2] = {0,0};
	long buttonTime[2] = { 0,0 };

	buttonMng.readButtons(buttonStatus,buttonTime);
	// Reads in the Button Status (NotPressed = 0, Pressed = 1, Hold = 2, Released =3) and the Time since the last Press/Release

	// Button 0 is BUTTON_SUN 1 is BUTTON_MOON
	// if BUTTON_SUN is Pressed (and we are sure it is no bounce)
	if(buttonStatus[0] == 1 && buttonTime[0] > BOUNCE_TIME)
	{
		//digitalWrite(GREEN, 1);
		
	}
	// when it is hold long enough
	else if (buttonStatus[0] == 2 && buttonTime[0] > HOLD_TIME)
	{
		//digitalWrite(YELLOW, 1);
		
	}
	// when it is released after Holding
	else if (buttonStatus[0] == 3 && buttonTime[0] > HOLD_TIME)
	{
		//digitalWrite(ORANGE, 1);
		
	}
	// when it is released without Holding
	else if (buttonStatus[0] == 3 && buttonTime[0] < HOLD_TIME && buttonTime[0] > BOUNCE_TIME)
	{
		//digitalWrite(RED, 1);
	}
	static long lastTimeClicked = 0;
	static uint8_t lightMode = 0;
	static bool off = false;
	// The same for BUTTON_MOON press
	if (buttonStatus[1] == 1 && buttonTime[1] > BOUNCE_TIME)
	{
		// Switch the light Mode when double Clicked
		if (millis() - lastTimeClicked < DOUBLE_CLICK_TIME)
		{
			off = false;
			lightMode++;
			lightMode %= LED_LIGHT_MODES;
			// we dont want a triple click as double click
			lastTimeClicked = 0;
		}
		else
		{
			lastTimeClicked = millis();
		}
	}
	// hold
	else if (buttonStatus[1] == 2 && buttonTime[1] > HOLD_TIME)
	{
		batteryCheck = true;
	}
	// hold release
	else if (buttonStatus[1] == 3 && buttonTime[1] > HOLD_TIME)
	{

	}
	// not hold release
	else if (buttonStatus[1] == 3 && buttonTime[1] < HOLD_TIME && buttonTime[1] > BOUNCE_TIME)
	{
		uint8_t shapeMatrix[8][6];
		frameChanged = true;

		// if it is off turn the lights off
		if (off)
		{
			RGBColor colors[] = { RGBColor(0,0,0) };
			Sketches::getSketch(shapeMatrix, Shapes::Filled);
			curFrame = Frame(shapeMatrix, colors, sizeof(colors));
			lastTimeClicked = millis();
		}
		else 
		{
			switch (lightMode)
			{
			/*case 0:
				{
					RGBColor colors[] = { RGBColor(0,0,0) };
					Sketches::getSketch(shapeMatrix, Shapes::Filled);
					curFrame = Frame(shapeMatrix, colors, sizeof(colors));
					break;
				}*/
			case 0:
				{
					RGBColor colors[] = { RGBColor(100,100,100) };
					Sketches::getSketch(shapeMatrix, Shapes::Filled);
					curFrame = Frame(shapeMatrix, colors, sizeof(colors));
					break;
				}
			case 1:
				{	
					RGBColor colors[] = { RGBColor(100,0,0) };
					Sketches::getSketch(shapeMatrix, Shapes::Filled);
					curFrame = Frame(shapeMatrix, colors, sizeof(colors));
					break;
				}
			case 2:
				{
					RGBColor colors[] = { RGBColor(0,100,0) };
					Sketches::getSketch(shapeMatrix, Shapes::Filled);
					curFrame = Frame(shapeMatrix, colors, sizeof(colors));
					break;
				}
			case 3:
				{
					RGBColor colors[] = { RGBColor(0,0,100) };
					Sketches::getSketch(shapeMatrix, Shapes::Filled);
					curFrame = Frame(shapeMatrix, colors, sizeof(colors));
					break;
				}
			case 4:
				{
					RGBColor colors[] = { RGBColor(0,0,0),RGBColor(100,0,25) };
					Sketches::getSketch(shapeMatrix, Shapes::Hearth);
					curFrame = Frame(shapeMatrix, colors, sizeof(colors));
					break;
				}
			case 5:
				{
					RGBColor colors[] = { RGBColor(0,0,0),RGBColor(20,20,0),RGBColor(20,20,20),RGBColor(40,0,0) };
					Sketches::getSketch(shapeMatrix, Shapes::Smiley);
					curFrame = Frame(shapeMatrix, colors, sizeof(colors));
					break;
				}
			default:
				{
					RGBColor colors[] = { RGBColor(100,100,100) };
					Sketches::getSketch(shapeMatrix, Shapes::Filled);
					curFrame = Frame(shapeMatrix, colors, sizeof(colors));
					break;
				}
			}
		}
		// Switch on/off
		off = !off;
	}

	return batteryCheck;
}
