/* flatlight revision 15 aka "AUM-Light"
 *  atmega 88 au int 8 MHz
 *  2 x sk6812 mini status 
 *  2 Buttons
 *  4 osram Powerleds
 *  
 *  TODO : 
 *  // half the blue value to be dimmer when the light is dim ?
 *  
 */

#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/interrupt.h>
#include <WS2812.h>
#include <math.h>
#include <SimpleTimer.h>

const int BUTTON_UP = 6;          // define the up button pin 
const int BUTTON_DOWN = 3;          // define the down button pin 


const int WHITELED = 10;                // PowerLED PWM control. (OC1C // OC0A // PB7)
int WHITELEDSTATE = 0;               // WHITELED off
int LIGHTSTATE = 0;                // set lightstate value

int up_lastReading = LOW;            // button read value
long up_onTime = 0;                  // button read ontime
long up_lastSwitchTime = 0;          // button switchtime value
int up_hold = 0;                     // button hold value
int up_single = 0;                   // button single press value

int down_lastReading = LOW;            // button read value
long down_onTime = 0;                  // button read ontime
long down_lastSwitchTime = 0;          // button switchtime value
int down_hold = 0;                     // button hold value
int down_single = 0;  


const int bounceTime = 50;              // define button debounce
const int holdTime = 250;               // define button hold time
const int doubleTime = 500;             // define button doubleclick time


#define PIXELCount 2             //WS2812 pixel count
#define outputPin 9               //WS2812 signal pin
WS2812 PIXEL(PIXELCount);
cRGB value;
/*
const byte dim_curve[] = {
  0, 1, 1, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3,
  3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4,
  4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 6, 6, 6,
  6, 6, 6, 6, 6, 7, 7, 7, 7, 7, 7, 7, 8, 8, 8, 8,
  8, 8, 9, 9, 9, 9, 9, 9, 10, 10, 10, 10, 10, 11, 11, 11,
  11, 11, 12, 12, 12, 12, 12, 13, 13, 13, 13, 14, 14, 14, 14, 15,
  15, 15, 16, 16, 16, 16, 17, 17, 17, 18, 18, 18, 19, 19, 19, 20,
  20, 20, 21, 21, 22, 22, 22, 23, 23, 24, 24, 25, 25, 25, 26, 26,
  27, 27, 28, 28, 29, 29, 30, 30, 31, 32, 32, 33, 33, 34, 35, 35,
  36, 36, 37, 38, 38, 39, 40, 40, 41, 42, 43, 43, 44, 45, 46, 47,
  48, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62,
  63, 64, 65, 66, 68, 69, 70, 71, 73, 74, 75, 76, 78, 79, 81, 82,
  83, 85, 86, 88, 90, 91, 93, 94, 96, 98, 99, 101, 103, 105, 107, 109,
  110, 112, 114, 116, 118, 121, 123, 125, 127, 129, 132, 134, 136, 139, 141, 144,
  146, 149, 151, 154, 157, 159, 162, 165, 168, 171, 174, 177, 180, 183, 186, 190,
  193, 196, 200, 203, 207, 211, 214, 218, 222, 226, 230, 234, 238, 242, 248, 255,
};

*/

// calculate temp in °C from ntc analog raw data
double Thermistor(int RawADC) {
  double Temp;
  Temp = log(10000.0 * ((1024.0 / RawADC - 1)));
  Temp = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * Temp * Temp )) * Temp );
  Temp = Temp - 273.15;
  return Temp;
}


//  check supply voltage
long readVcc() {
ADMUX = _BV(REFS0)  | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
delay(4);                           // Wait for Vref to settle
ADCSRA |= _BV(ADSC);                // Start conversion
while (bit_is_set(ADCSRA, ADSC));   // measuring
uint8_t low  = ADCL;                // must read ADCL first - it then locks ADCH
uint8_t high = ADCH;                // unlocks both
long result = (high << 8) | low;
result = 1105000L / result;         // Calculate Vcc, 1.23 V Bandgap +/- 10 % -- ADJUST !!
return result;                      // Vcc in millivolts
}

SimpleTimer timer;


void setup() {

//  Serial.begin(9600);
//  Serial.println(x);



  timer.setInterval(120000, BATCHECK);               // call VTCHECK every 120 seconds

  //define PIN Modes
  pinMode(BUTTON_UP, INPUT);
  pinMode(BUTTON_DOWN, INPUT);

  pinMode(WHITELED, OUTPUT);                      // pocketsun
  
  // FastPWM @ 4MHz for the WHITELED 
  TCCR1A =  _BV(WGM10)  | _BV(WGM12);
  TCCR1B = _BV(CS10) ;

  // ws2812_light
  PIXEL.setOutput(outputPin);
  PIXEL.setColorOrderGRB(); // for sk6812

  
  // setup LEDS
  TEMPBLANK();
  delay(10);
  BATCHECK();
  delay(10);
  WHITELEDSTATE = 20; // start at moderate brightness
  analogWrite(WHITELED, WHITELEDSTATE);
      value.b = 240 - WHITELEDSTATE;
      value.g = 0;
      value.r = WHITELEDSTATE;   
      PIXEL.set_crgb_at(0, value);   
      PIXEL.sync();
  delay(10);

  // check voltage

}

// start main loop
void loop() {

  timer.run();                                    // start timer

// temp check
  int val;                
  double temp;            
  val=analogRead(0);      
  temp=Thermistor(val); 

  while(temp > 75){
    // reduce brightness blink eside templed for warning etc
    analogWrite(WHITELED, 25);
    TEMPWARN();
    delay(50);
    TEMPBLANK();
    delay(250);
    TEMPWARN();
    delay(50);
    TEMPBLANK();
    delay(500);
    // double check
    int val;                
    double temp;            
    val=analogRead(0);      
    temp=Thermistor(val); 
    // if temperatur falls below 50°C go to 50% brightness
    if (temp < 40){
    // something here , restore some lightmode
      WHITELEDSTATE = 25 ;
      analogWrite(WHITELED, WHITELEDSTATE);
      value.b = 240 - WHITELEDSTATE;
      value.g = 0;
      value.r = WHITELEDSTATE;   
      PIXEL.set_crgb_at(0, value);   
      PIXEL.sync();
      break;
    }
  }

  // Button UP routine
  int up_reading = digitalRead(BUTTON_UP);

  //button first pressed
  if (up_reading == HIGH && up_lastReading == LOW) {
    up_onTime = millis();
  }

  //button held !
  if (up_reading == HIGH && up_lastReading == HIGH) {
    if ((millis() - up_onTime) > holdTime) {

      // do something when held
    WHITELEDSTATE_UP();

      up_hold = 1;

    }
  }

  // button released (is it ?)

  if (up_reading == LOW && up_lastReading == HIGH) {
    if (((millis() - up_onTime) > bounceTime) && up_hold != 1) {
      onUPRelease();
    }

    if (up_hold == 1) {

      // do something if released after hold

      up_hold = 0;
    }
  }

  up_lastReading = up_reading;

  if (up_single == 1 && (millis() - up_lastSwitchTime) > doubleTime) {

    // button single click
     WHITELEDSTATE_UP();

    up_single = 0;
  }

  // Button UP END
  // Button DOWN START

  int down_reading = digitalRead(BUTTON_DOWN);

  //button first pressed
  if (down_reading == HIGH && down_lastReading == LOW) {
    down_onTime = millis();
  }

  //button held !
  if (down_reading == HIGH && down_lastReading == HIGH) {
    if ((millis() - down_onTime) > holdTime) {

      // do something on hold

    WHITELEDSTATE_DOWN();

      down_hold = 1;
    }
  }

  // button released (is it ?)

  if (down_reading == LOW && down_lastReading == HIGH) {
    if (((millis() - down_onTime) > bounceTime) && down_hold != 1) {
      onDOWNRelease();
    }

    if (down_hold == 1) {

      // do something if released after hold

      down_hold = 0;
    }
  }

  down_lastReading = down_reading;

  if (down_single == 1 && (millis() - down_lastSwitchTime) > doubleTime) {

    // button single click

    WHITELEDSTATE_DOWN();

    down_single = 0;
  }
  // BUTTON DOWN END


}




//onDOWNRelease button function

void onDOWNRelease() {

  if ((millis() - down_lastSwitchTime) >= doubleTime) {
    down_single = 1;
    down_lastSwitchTime = millis();
    return;
  }

  if ((millis() - down_lastSwitchTime) < doubleTime) {

    // do something on double click
    READBOOK();
    
    down_single = 0;
    down_lastSwitchTime = millis();
  }
}


//onUPRelease button function

void onUPRelease() {

  if ((millis() - up_lastSwitchTime) >= doubleTime) {
    up_single = 1;
    up_lastSwitchTime = millis();
    return;
  }
  if ((millis() - up_lastSwitchTime) < doubleTime) {


    // do something on double click
    FLASH();


    up_single = 0;
    up_lastSwitchTime = millis();
  }
}


void WHITELEDSTATE_UP() {
  while(true){
  if (WHITELEDSTATE >= 240) {
    WHITELEDSTATE = 240;
    analogWrite(WHITELED, WHITELEDSTATE);
      value.b = 240 - WHITELEDSTATE;  // half the blue value to be dimmer when the light is dim ?
      value.g = 0;
      value.r = WHITELEDSTATE;   
      PIXEL.set_crgb_at(0, value);   
      PIXEL.sync();
  } else {
    WHITELEDSTATE = WHITELEDSTATE + 2;
    analogWrite(WHITELED, WHITELEDSTATE);
      value.b = 240 - WHITELEDSTATE;
      value.g = 0;
      value.r = WHITELEDSTATE;   
      PIXEL.set_crgb_at(0, value);   
      PIXEL.sync();
    delay(30);
  }
  int kitkat = digitalRead(BUTTON_UP);
    if (kitkat == LOW) {
      break;
    }
  }
}


void WHITELEDSTATE_DOWN() {
  while(true){
  if (WHITELEDSTATE <= 10) {
    WHITELEDSTATE = 0;
    analogWrite(WHITELED, WHITELEDSTATE);
      value.b = 240 - WHITELEDSTATE;
      value.g = 0;
      value.r = WHITELEDSTATE;   
      PIXEL.set_crgb_at(0, value);   
      PIXEL.sync();
  } else {
    WHITELEDSTATE = WHITELEDSTATE - 10;
    analogWrite(WHITELED, WHITELEDSTATE);
      value.b = 240 - WHITELEDSTATE;
      value.g = 0;
      value.r = WHITELEDSTATE;   
      PIXEL.set_crgb_at(0, value);   
      PIXEL.sync();
    delay(50);
  }
  int kitkat = digitalRead(BUTTON_DOWN);
    if (kitkat == LOW) {
      break;
    }
  }
}

void BATCHECK() {

  analogWrite(WHITELED, LOW);         // OFF the light (the load drops the voltage extremely, its analog, right ;))

  uint16_t  mv = readVcc();           

  if (mv < 2700){
      value.b = 0;
      value.g = 0;
      value.r = 200;
  }else if (mv > 2700 && mv < 3000){
      value.b = 0;
      value.g = 20;
      value.r = 200;  
  }else if (mv > 3000 && mv < 3200){
      value.b = 0;
      value.g = 20;
      value.r = 180;  
  }else if (mv > 3200 && mv < 3400){
      value.b = 0;
      value.g = 70;
      value.r = 140;  
  }else if (mv > 3400 && mv < 3600){
      value.b = 0;
      value.g = 100;
      value.r = 100;  
  }else if (mv > 3600 && mv < 3700){
      value.b = 0;
      value.g = 150;
      value.r = 60; 
  }else if (mv > 3700 && mv < 3800){
      value.b = 0;
      value.g = 170;
      value.r = 30; 
  }else if (mv > 3800 && mv < 3900){
      value.b = 0;
      value.g = 200;
      value.r = 10;   
  }else if (mv > 4000){
      value.b = 10;
      value.g = 250;
      value.r = 0;  
  }    
      PIXEL.set_crgb_at(1, value);   
      PIXEL.sync();

  analogWrite(WHITELED, WHITELEDSTATE);   // restore the light
}



void TEMPWARN() {
      value.b = 0;
      value.g = 0;
      value.r = 200;
      PIXEL.set_crgb_at(0, value);                  // Set value at LED found at index 0
      PIXEL.sync();
}

void TEMPBLANK() {
    value.b = 0;
    value.g = 0;
    value.r = 0;
    PIXEL.set_crgb_at(0, value);
    PIXEL.sync();
}

void READBOOK() {
  uint16_t  mv = readVcc();             
  if (mv > 3900){
    WHITELEDSTATE = 2;
    } 
  else if(mv > 3500 && mv < 3900) {
    WHITELEDSTATE = 3;
    }
  else if(mv > 3000 && mv < 3500) {
    WHITELEDSTATE = 4;
    }
  else if(mv < 3000) {
    WHITELEDSTATE = 5;
    }
  
  while (true){

    analogWrite(WHITELED, WHITELEDSTATE);   
    value.b = 0;
    value.g = 0;
    value.r = 0;  
    PIXEL.set_crgb_at(0, value);   
    PIXEL.set_crgb_at(1, value);   
    PIXEL.sync();
  int kitkat = digitalRead(BUTTON_DOWN);
    if (kitkat == HIGH) {
      BATCHECK();
      value.b = 240 - WHITELEDSTATE;
      value.g = 0;
      value.r = WHITELEDSTATE;   
      PIXEL.set_crgb_at(0, value);   
      PIXEL.sync();
      break;
    }
  }
}

void FLASH(){
 
//  while (true){

  uint16_t  FLASHLEDSTATE = 230;
  uint16_t  mv = readVcc();             
  if (mv > 3900){
    FLASHLEDSTATE = 230;
    } 
  else if(mv > 3500 && mv < 3900) {
    FLASHLEDSTATE = 240;
    }
  else if(mv > 3000 && mv < 3500) {
    FLASHLEDSTATE = 250;
    }
  else if(mv < 3000) {
    FLASHLEDSTATE = 255;
    }
  

    analogWrite(WHITELED, FLASHLEDSTATE);
    delay(2500);
    analogWrite(WHITELED, WHITELEDSTATE);
    
/*  int kitkat = digitalRead(BUTTON_UP);
    if (kitkat == HIGH) {
      BATCHECK();
      WHITELEDSTATE = 25;
      analogWrite(WHITELED, WHITELEDSTATE);
      value.b = 240 - WHITELEDSTATE;
      value.g = 0;
      value.r = WHITELEDSTATE;   
      PIXEL.set_crgb_at(0, value);   
      PIXEL.sync();
      break;
    }
  }
*/
}
