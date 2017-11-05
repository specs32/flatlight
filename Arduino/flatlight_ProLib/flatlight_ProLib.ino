// neopixel flatlight with atmega 8 / 88 / 16 @ 8MHz

#include <WS2812.h>
#include <math.h>
#include <ProLibMain.h>
#include <TemperatureManager.h>

// define PINS and other variables

const int button = 9;           // define button pin
int bounceTime = 50;              // define button debounce
int holdTime = 250;               // define button hold time
int doubleTime = 500;             // define button doubleclick time
int lastReading = LOW;            // button read value
int hold = 0;                     // button hold value
int single = 0;                   // button single press value
long onTime = 0;                  // button read ontime
long lastSwitchTime = 0;          // button switchtime value

int lightMode = 0;                // set lightmode value
int lightState = 0;                // set lightstate value

const int xbutton = 8;          // define the other button pin ^^
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
const int xled = 12;                // Cree (X)LEDs

long steps = 0;                  // value for smooth fading

const int RED = 17;               // Battery Indicator Pin
const int ORANGE = 18;            // Battery Indicator Pin
const int YELLOW = 19;            // Battery Indicator Pin
const int GREEN = 20;             // Battery Indicator Pin
const int BRIGHTGREEN = 21;       // Battery Indicator Pin
const int templed = 22;

#define LEDCount 48
#define outputPin 13

static ProLibMain proLib;
WS2812 PIXEL(LEDCount);
cRGB value;

/*// calculate temp in °C from ntc (just to make the values clearer(for me))
double Thermistor(int RawADC) {
  double Temp;
  Temp = log(10000.0 * ((1024.0 / RawADC - 1)));
  Temp = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * Temp * Temp )) * Temp );
  Temp = Temp - 273.15;
  return Temp;
}*/


//  check supply voltage
long readVcc() {

  ADMUX = _BV(REFS0) | _BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);

  delay(4);                           // Wait for Vref to settle
  ADCSRA |= _BV(ADSC);                // Start conversion
  while (bit_is_set(ADCSRA, ADSC));   // measuring

  uint8_t low  = ADCL;                // must read ADCL first - it then locks ADCH
  uint8_t high = ADCH;                // unlocks both

  long result = (high << 8) | low;

  result = 1225300L / result;         // Calculate Vcc <--- ADJUST since it can be 10 % off (default 1.23V)

  return result;                      // Vcc in millivolts
}

// well .. guess
void setup() {

  //define PIN Modes

  pinMode(button, INPUT);
  pinMode(xbutton, INPUT);
  pinMode(templed, OUTPUT);
  pinMode(RED, OUTPUT);
  pinMode(ORANGE, OUTPUT);
  pinMode(YELLOW, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BRIGHTGREEN, OUTPUT);


  PIXEL.setOutput(outputPin);
  PIXEL.setColorOrderGRB(); // for sk6812 rgbw pixel
  digitalWrite(RED, 1); // For Debugging
  ProLibMain proLib = ProLibMain::getInstance();
  digitalWrite(ORANGE, 1);  // For Debugging
  //Battery Check on Startup
  BATcheck();
  delay(2000);
  BATclear();
}


// start main loop
void loop() {
  // read and store temp
  ADMUX = _BV(REFS0) |_BV(REFS1);
  proLib.proTemperature();
  // Button routine
  while(1){
    proLib.proButtons();
  }

  /*int reading = digitalRead(button);

  //button first pressed
  if (reading == HIGH && lastReading == LOW) {
    onTime = millis();
  }

  //button held (as long as held switch all other LEDs off to get a real VCC value to read out.. no drag)
  if (reading == HIGH && lastReading == HIGH) {
    if ((millis() - onTime) > holdTime) {


      blank();  //blank both matrix
      xblank(); //and XLED
      BATcheck (); // display battery status on hold

      hold = 1;
    }
  }

  // button released (is it ?)

  if (reading == LOW && lastReading == HIGH) {
    if (((millis() - onTime) > bounceTime) && hold != 1) {
      onRelease();
    }

    if (hold == 1) {
      // if released after hold, restore normal operation

      BATclear(); // clear on release

      hold = 0;
    }
  }

  lastReading = reading;

  if (single == 1 && (millis() - lastSwitchTime) > doubleTime) {

    // button single click (set matrix brightness)

    LEDSTATE();
    single = 0;
  }

  //BUTTON ROUTINE END
  // *******
  // XBUTTON ROUTINE START


  int xreading = digitalRead(xbutton);

  //button first pressed
  if (xreading == HIGH && xlastReading == LOW) {
    xonTime = millis();
  }

  //button held
  if (xreading == HIGH && xlastReading == HIGH) {
    if ((millis() - xonTime) > xholdTime) {

      blank();
      //xblank();
      //BATcheck (); // do something

      xhold = 1;
    }
  }

  // button released (is it ?)

  if (xreading == LOW && xlastReading == HIGH) {
    if (((millis() - xonTime) > xbounceTime) && xhold != 1) {
      xonRelease();
    }

    if (xhold == 1) {
      // if released after hold, restore normal operation

      BATclear(); // do something else

      xhold = 0;
    }
  }

  xlastReading = xreading;

  if (xsingle == 1 && (millis() - xlastSwitchTime) > xdoubleTime) {

    // button single click (set brightness XLED)

    XLEDSTATE();
    xsingle = 0;
  }*/
}

// Xroutine END

// Xonrelease button function (XLED)
void xonRelease() {

  // was it not a single click ?

  if ((millis() - xlastSwitchTime) >= xdoubleTime) {
    xsingle = 1;
    xlastSwitchTime = millis();
    return;
  }

  // was it a double click ?

  if ((millis() - xlastSwitchTime) < xdoubleTime) {

    //      do something here

    xsingle = 0;
    xlastSwitchTime = millis();
  }
}

//onrelease button function (matrix)
void onRelease() {

  // was it not a single click ?

  if ((millis() - lastSwitchTime) >= doubleTime) {
    single = 1;
    lastSwitchTime = millis();
    return;
  }

  // was it a double click ?

  if ((millis() - lastSwitchTime) < doubleTime) {

    LEDMODE();
    single = 0;
    lastSwitchTime = millis();
  }
}


// Main LEDs matrix brightness function
void LEDSTATE() {

  lightState = lightState + 1;

  if (lightState == 5) {
    lightState = 1;
  }

  if (lightState == 1) {

    for (uint16_t i = 0; i < LEDCount; i++) {
      value.b = 0;
      value.g = 0;
      value.r = 0; // RGB Value -> Green Only
      PIXEL.set_crgb_at(i, value); // Set value at LED found at index 0
    }
    PIXEL.sync();

  } else if (lightState == 2) {

    for (uint16_t i = 0; i < LEDCount; i++) {
      value.b = 20;
      value.g = 20;
      value.r = 20; // RGB Value -> Green Only
      PIXEL.set_crgb_at(i, value); // Set value at LED found at index 0
    }
    PIXEL.sync();

  } else if (lightState == 3) {

    for (uint16_t i = 0; i < LEDCount; i++) {
      value.b = 100;
      value.g = 100;
      value.r = 100; // RGB Value -> Green Only
      PIXEL.set_crgb_at(i, value); // Set value at LED found at index 0
    }
    PIXEL.sync();
  }

  else if (lightState == 4) {

    for (uint16_t i = 0; i < LEDCount; i++) {
      value.b = 220;
      value.g = 220;
      value.r = 220; // RGB Value -> Green Only
      PIXEL.set_crgb_at(i, value); // Set value at LED found at index 0
    }
    PIXEL.sync();
  }
}


// LEDMODE matrix colours, patterns, etc.
void LEDMODE() {


  lightMode = lightMode + 1;

  if (lightMode == 6) {
    lightMode = 1;
  }

  if (lightMode == 1) {

    for (uint16_t i = 0; i < LEDCount; i++) {
      value.b = 100;
      value.g = 0;
      value.r = 0;
      PIXEL.set_crgb_at(i, value);
    }
    PIXEL.sync();

  } else if (lightMode == 2) {

    for (uint16_t i = 0; i < LEDCount; i++) {
      value.b = 0;
      value.g = 0;
      value.r = 100;
      PIXEL.set_crgb_at(i, value);
    }
    PIXEL.sync();

  } else if (lightMode == 3) {

    for (uint16_t i = 0; i < LEDCount; i++) {
      value.b = 0;
      value.g = 100;
      value.r = 0;
      PIXEL.set_crgb_at(i, value);
    }
    PIXEL.sync();
  }

  else if (lightMode == 4) {

    // uh, a nice pink heart

    /*for (uint16_t i = 0; i < LEDCount; i++) {
      for (uint16_t x = 0; x < 160; x++) {
        value.b = hearts[x];
        x++;
        value.g = hearts[x];
        x++;
        value.r = hearts[x];
        PIXEL.set_crgb_at(i, value);
        i++;
      }
    }
    PIXEL.sync();
  }

  else if (lightMode == 5) {

    // smile !

    for (uint16_t i = 0; i < LEDCount; i++) {
      for (uint16_t x = 0; x < 160; x++) {
        value.b = smiley[x];
        x++;
        value.g = smiley[x];
        x++;
        value.r = smiley[x];
        PIXEL.set_crgb_at(i, value);
        i++;
      }
    }
    PIXEL.sync();*/
  }
}


// (X)LEDS brightness function

void XLEDSTATE() {

  xlightMode = xlightMode + 1;

  if (xlightMode == 5)
  {
    xlightMode = 1;
  }
  if (xlightMode == 1)
  {
    XLEDstate = 0;
    fade();
    xpreviousLEDstate = XLEDstate;
  } else if (xlightMode == 2) {
    XLEDstate = 8;
    fade();
    xpreviousLEDstate = XLEDstate;
  } else if (xlightMode == 3) {
    XLEDstate = 127;
    fade();
    xpreviousLEDstate = XLEDstate;
  }
  else if (xlightMode == 4)
  {
    XLEDstate = 234;
    fade();
    xpreviousLEDstate = XLEDstate;
  }
}


// battery check routine PC1-PC5 LEDs
void BATcheck() {

  uint16_t  mv = readVcc();

  if (mv < 3200) {
    digitalWrite (RED, 1);
    digitalWrite (ORANGE, 0);
    digitalWrite (YELLOW, 0);
    digitalWrite (GREEN, 0);
    digitalWrite (BRIGHTGREEN, 0);

  }

  else if (mv > 3200 && mv < 3400) {
    digitalWrite (RED, 1);
    digitalWrite (ORANGE, 1);
    digitalWrite (YELLOW, 0);
    digitalWrite (GREEN, 0);
    digitalWrite (BRIGHTGREEN, 0);

  }

  else if (mv > 3400 && mv < 3600) {
    digitalWrite (RED, 1);
    digitalWrite (ORANGE, 1);
    digitalWrite (YELLOW, 1);
    digitalWrite (GREEN, 0);
    digitalWrite (BRIGHTGREEN, 0);

  }

  else if (mv > 3600 && mv < 4000) {
    digitalWrite (RED, 1);
    digitalWrite (ORANGE, 1);
    digitalWrite (YELLOW, 1);
    digitalWrite (GREEN, 1);
    digitalWrite (BRIGHTGREEN, 0);
  }

  else if (mv > 4000) {
    digitalWrite (RED, 1);
    digitalWrite (ORANGE, 1);
    digitalWrite (YELLOW, 1);
    digitalWrite (GREEN, 1);
    digitalWrite (BRIGHTGREEN, 1);

  }
}

//clear PC1-PC5
void BATclear() {
  digitalWrite (RED, 0);
  digitalWrite (ORANGE, 0);
  digitalWrite (YELLOW, 0);
  digitalWrite (GREEN, 0);
  digitalWrite (BRIGHTGREEN, 0);
}


//blank XLED

void xblank() {
  analogWrite(xled, 0);
}

//blank Matrix
void blank() {
  for (uint16_t i = 0; i < LEDCount; i++) {
    value.b = 0;
    value.g = 0;
    value.r = 0;
    PIXEL.set_crgb_at(i, value);
  }
  PIXEL.sync();
}

//fade XLED
void fade() {
  steps = 128 / XLEDstate;
  for (int i = xpreviousLEDstate; i <= XLEDstate; i++) {
    analogWrite(xled, i);
    delay(steps);
  }
  for (int i = xpreviousLEDstate; i >= XLEDstate; i--) {
    analogWrite(xled, i);
    delay(steps / 8);
  }
}

// end of file
// end of file
// end of file



