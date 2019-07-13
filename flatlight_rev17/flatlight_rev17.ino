/* flatlight revision 17 aka (small) "DRAGON-Light"
 *  butchered together by http://github.com/specs32
 *  atmega 328 QFP - 8 MHZ intern OSC !!
 *  EX: % avrdude -e -c usbasp -pm328 -B10 -U lfuse:w:0xe2:m -U hfuse:w:0xd9:m -U efuse:w:0xff:m 
 *  2 x APA102 - 2020  status-leds
 *  2 logic Buttons (use internal pullup !)
 *  1 OLED
 *  1 IMU
 *  
 *  This code version is for the LED-SIDE REV 17 with :
 *  12 Cree J-Series HighPower-Leds and a DIO5116 LED driver !
 *  
 *  
 *  TODO : wrtite your own code !
 *  freaking mosfets !  
 */

// TEMP



 
// Use qsuba for smooth pixel colouring and qsubd for non-smooth pixel colouring
#define qsubd(x, b)  ((x>b)?b:0)                              // Digital unsigned subtraction macro. if result <0, then => 0. Otherwise, take on fixed value.
#define qsuba(x, b)  ((x>b)?x-b:0)                            // Analog Unsigned subtraction macro. if result <0, then => 0

#include <U8g2lib.h>
U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R2, U8X8_PIN_NONE);   //  display config

#include <FastLED.h>
#define LED_DT 6                                             // Serial data pin for WS2812 or WS2801.
#define LED_CK 5                                             // Serial clock pin for WS2801 or APA102.
#define COLOR_ORDER BGR                                       // Are they GRB for WS2812 and GBR for APA102
#define LED_TYPE APA102                                       // What kind of strip are you using? WS2812, APA102. . .
#define NUM_LEDS 2      
struct CRGB leds[NUM_LEDS];                                   // Initialize our LED array.
CRGBPalette16 currentPalette;                                 // Palette definitions
CRGBPalette16 targetPalette;
TBlendType currentBlending = LINEARBLEND;
uint8_t batled = 1;                                           //name the pixels
uint8_t briled = 0;
uint8_t max_bright = 255;                                     // Overall brightness definition. It can be changed on the fly.

#include "SparkFunLSM6DS3.h" // gyro
LSM6DS3 myIMU( I2C_MODE, 0x6A );  //I2C device address 0x6A
#define int1Pin 3  //Use pin 2 for int.0 on uno
uint8_t int1Status = 0;

#include <SimpleTimer.h>              // out of the arduino repo 

const int PWMLIMIT = 255;             // set maximum duty cycle adjust for your needs
const int BUTTON_UP = 16;             // define the up button pin 
const int BUTTON_DOWN = 17;           // define the down button pin 
const int WHITELED = 10;              // PowerLED LED-DRIVER (!) PWM control. (OC1B // PB2 // PIN10)
int WHITELEDSTATE = 1;                // WHITELED @ 1(255)

int up_lastReading = HIGH;            // button read value
long up_onTime = 0;                   // button read ontime
long up_lastSwitchTime = 0;           // button switchtime value
int up_hold = 0;                      // button hold value
int up_single = 0;                    // button single press value

int down_lastReading = HIGH;          // button read value
long down_onTime = 0;                 // button read ontime
long down_lastSwitchTime = 0;         // button switchtime value
int down_hold = 0;                    // button hold value
int down_single = 0;  

const int bounceTime = 50;             // define button debounce
const int holdTime = 250;              // define button hold time
const int doubleTime = 700;            // define button doubleclick time

//  check supply voltage
long readVcc() {
ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);   //  _BV(REFS1) | <-- insert this to measure bandgap voltage (m328)
//delay(10);                           // Wait for Vref
ADCSRA |= _BV(ADSC);                // Start conversion
while (bit_is_set(ADCSRA, ADSC));   // measuring
uint8_t low  = ADCL;                // must read ADCL first - it then locks ADCH
uint8_t high = ADCH;                // unlocks both
long result = (high << 8) | low;
result = 1170000L / result;         // Calculate Vcc, Bandgap +/- 10 % -- ADJUST !!
return result;                      // Vcc in millivolts
}

SimpleTimer zoled;

void SLEEPOLED(){
  u8g2.setPowerSave(1);                                     // put oled to sleep
}

void setup() {
  // wait for it ....
  delay(1000);
  // there you go ;)
  
  //define PIN Modes
  pinMode(BUTTON_UP, INPUT_PULLUP);
  pinMode(BUTTON_DOWN, INPUT_PULLUP);
  pinMode(WHITELED, OUTPUT); 
    
  // Light first !
  analogWrite(WHITELED, WHITELEDSTATE);
  leds[briled].b = (255 - WHITELEDSTATE)/2;               // set briled pixel to initial value
  leds[briled].g = 0;
  leds[briled].r = WHITELEDSTATE/2; 
  leds[batled].b = 55;                                    // set barled pixel to initial value
  leds[batled].g = 55;
  leds[batled].r = 0; 
  FastLED.show();

  // setup pixels (bgr!)
  LEDS.addLeds<LED_TYPE, LED_DT, LED_CK, COLOR_ORDER>(leds, NUM_LEDS);  //WS2801 and APA102
  FastLED.setBrightness(max_bright);
  currentPalette = OceanColors_p;

  // Start the OLED
  u8g2.begin();

  // Show Voltage / Brightness on display
  BATDISP();                             

  // Oled Sleep Timer
  zoled.setInterval(5000, SLEEPOLED);               // call  every x milliseconds

// Experiments in :

// Power saving 
//    PRR =  _BV(PRUSART0) | _BV(PRTWI) | _BV(PRSPI); //_BV(PRTIM2) | _BV(PRTIM0) |
//    MCUCR = _BV(BODSE) | _BV(BODS);
//  Serial.begin(9600);
//  Serial.println(x);

// FastPWM @ 4MHz
//  TCCR1A =  _BV(COM1A1) | _BV(WGM11);
//  TCCR1B = _BV(CS10) | _BV(WGM12);



} // setup end



void loop() {

 zoled.run();                         // start timer

  // Button UP routine
  int up_reading = digitalRead(BUTTON_UP);
  if (up_reading == LOW && up_lastReading == HIGH) {
    up_onTime = millis();
  }
  if (up_reading == LOW && up_lastReading == LOW) {
    if ((millis() - up_onTime) > holdTime) {
    WHITELEDSTATE_UP();                                 // do something when held
      up_hold = 1;
    }
  }
  if (up_reading == HIGH && up_lastReading == LOW ) {
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
     WHITELEDSTATE_UP();                                    // button single click
    up_single = 0;
  }
  // Button UP END

  // Button DOWN START
  int down_reading = digitalRead(BUTTON_DOWN);
  if (down_reading == LOW && down_lastReading == HIGH) {
    down_onTime = millis();
  }
  if (down_reading == LOW && down_lastReading == LOW) {
    if ((millis() - down_onTime) > holdTime) {
    WHITELEDSTATE_DOWN();                                     // do something on hold
      down_hold = 1;
    }
  }

  if (down_reading == HIGH && down_lastReading == LOW) {
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
    WHITELEDSTATE_DOWN();                               // button single click
    down_single = 0;
  }
  // BUTTON DOWN END


  if (WHITELEDSTATE == 0){
   while (true){

    u8g2.setPowerSave(0);                                     // wake  up display
    u8g2.setFont(u8g2_font_ncenB08_tr);               // u8g2_font_ncenB08_tr ,
    u8g2.clearBuffer();
    u8g2.drawStr(0,8,"PLASMA !!! ");
    u8g2.sendBuffer();
    
    EVERY_N_MILLISECONDS(25) {                                  // FastLED based non-blocking delay to update/display the sequence.
      plasma();
      }
    EVERY_N_MILLISECONDS(500) {
      uint8_t maxChanges = 56; 
      nblendPaletteTowardPalette(currentPalette, targetPalette, maxChanges);   // AWESOME palette blending capability.
      }
    EVERY_N_SECONDS(50) {                                 // Change the target palette to a random one every 5 seconds.
      uint8_t baseC = random8();                         // You can use this as a baseline colour if you want similar hues in the next line.
      targetPalette = CRGBPalette16(CHSV(baseC+random8(32), 192, random8(128,255)), CHSV(baseC+random8(32), 255, random8(128,255)), CHSV(baseC+random8(32), 192, random8(128,255)), CHSV(baseC+random8(32), 255, random8(128,255)));
      }
   
    FastLED.show();
    
    int kitkat = digitalRead(BUTTON_UP);
    if (kitkat == LOW) {
      leds[briled].b = 255 - WHITELEDSTATE /2;
      leds[briled].g = 0;
      leds[briled].r = WHITELEDSTATE /2;   
      FastLED.show();
      break;
    }
   }
 }


} // loop end


//onDOWNRelease button function
void onDOWNRelease() {

  if ((millis() - down_lastSwitchTime) >= doubleTime) {
    down_single = 1;
    down_lastSwitchTime = millis();
    return;
  }
  if ((millis() - down_lastSwitchTime) < doubleTime) {
    READBOOK();                             // do something on double click
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
    //BATDISP();                    // do something on double click
    FLASH();
    up_single = 0;
    up_lastSwitchTime = millis();
  }
}

// Brightness routines //
void WHITELEDSTATE_UP() {
  while(true){
  if (WHITELEDSTATE >= PWMLIMIT-5) {
    WHITELEDSTATE = PWMLIMIT;
    analogWrite(WHITELED, WHITELEDSTATE);
    BATDISP();
      leds[briled].b = (255 - WHITELEDSTATE);  // half the blue value to be dimmer when the light is dim ?
      leds[briled].g = 0;
      leds[briled].r = WHITELEDSTATE;   
      FastLED.show();
  } else {
    WHITELEDSTATE = WHITELEDSTATE + 5;
    analogWrite(WHITELED, WHITELEDSTATE);
    BATDISP();
      leds[briled].b = (255 - WHITELEDSTATE);  // half the blue value to be dimmer when the light is dim ?
      leds[briled].g = 0;
      leds[briled].r = WHITELEDSTATE;   
      FastLED.show();
    delay(30);
  }
  int kitkat = digitalRead(BUTTON_UP);
    if (kitkat == HIGH) {
      break;
      BATDISP();
    }
  }
}


void WHITELEDSTATE_DOWN() {
  while(true){
  if (WHITELEDSTATE <= 2) {
    WHITELEDSTATE = 0;
    analogWrite(WHITELED, WHITELEDSTATE);
    BATDISP();
      leds[briled].b = (255 - WHITELEDSTATE);
      leds[briled].g = 0;
      leds[briled].r = WHITELEDSTATE;   
      FastLED.show();
  } else {
    WHITELEDSTATE = WHITELEDSTATE - 2;
    analogWrite(WHITELED, WHITELEDSTATE);
    BATDISP();
      leds[briled].b = (255 - WHITELEDSTATE);  
      leds[briled].g = 0;
      leds[briled].r = WHITELEDSTATE;   
      FastLED.show();
    delay(50);
  }
  int kitkat = digitalRead(BUTTON_DOWN);
    if (kitkat == HIGH) {
      break;
      BATDISP();
    }
  }
}

//OLED voltage and brightness
void BATDISP(){                                             // all these values depend on the load and all the electronic components in the way, so .. its a vague approximation at best
 
  uint16_t mv = readVcc();                                  // check voltage under load !
  float xpercent = (float)WHITELEDSTATE / PWMLIMIT * 100;   // calculate brightness in percent
 
  u8g2.setPowerSave(0);                                     // wake  up display
  u8g2.setFont(u8g2_font_ncenB08_tr);                       // u8g2_font_ncenB08_tr ,
  u8g2.clearBuffer();  
  u8g2.drawStr(0,8,"Brightness: ");
  u8g2.setCursor(82, 8);
  u8g2.print(xpercent);                                     // print brightness
  u8g2.drawStr(0,20,"Voltage: ");
  u8g2.setCursor(82, 20);
  u8g2.print(mv);                                           // print voltage
  u8g2.sendBuffer();

  if (mv < 2700){                                           
      leds[batled].b = 0;
      leds[batled].g = 0;
      leds[batled].r = 120;
  }else if (mv > 2700 && mv < 3000){
      leds[batled].b = 0;
      leds[batled].g = 40;
      leds[batled].r = 120;  
  }else if (mv > 3000 && mv < 3200){
      leds[batled].b = 0;
      leds[batled].g = 80;
      leds[batled].r = 120;  
  }else if (mv > 3200 && mv < 3400){
      leds[batled].b = 0;
      leds[batled].g = 120;
      leds[batled].r = 120;  
  }else if (mv > 3400 && mv < 3600){
      leds[batled].b = 0;
      leds[batled].g = 120;
      leds[batled].r = 40;  
  }else if (mv > 3600 && mv < 3700){
      leds[batled].b = 0;
      leds[batled].g = 120;
      leds[batled].r = 0; 
  }else if (mv > 3700 && mv < 3800){
      leds[batled].b = 40;
      leds[batled].g = 120;
      leds[batled].r = 0; 
  }else if (mv > 3800 && mv < 3950){
      leds[batled].b = 80;
      leds[batled].g = 120;
      leds[batled].r = 0;   
  }else {
      leds[batled].b = 100;
      leds[batled].g = 100;
      leds[batled].r = 100;  
  }    
  FastLED.show();

}

// Nice plasma on the pixels please ! 
void plasma() {                                                 // This is the heart of this program. Sure is short. . . and fast.
  int thisPhase = beatsin8(6,-64,64);                           // Setting phase change for a couple of waves.
  int thatPhase = beatsin8(7,-64,64);
  for (int k=0; k<NUM_LEDS; k++) {                              // For each of the LED's in the strand, set a brightness based on a wave as follows:
    int colorIndex = cubicwave8((k*23)+thisPhase)/2 + cos8((k*15)+thatPhase)/2;           // Create a wave and add a phase change and add another wave with its own phase change.. Hey, you can even change the frequencies if you wish.
    int thisBright = qsuba(colorIndex, beatsin8(7,0,96));                                 // qsub gives it a bit of 'black' dead space by setting sets a minimum value. If colorIndex < current value of beatsin8(), then bright = 0. Otherwise, bright = colorIndex..
    leds[k] = ColorFromPalette(currentPalette, colorIndex, thisBright, currentBlending);  // Let's now add the foreground colour.
  }
} // plasma()


// Enouhg to read and last as long as possible
void READBOOK() {
    WHITELEDSTATE = 1;                                    //lowest PWM setting
    analogWrite(WHITELED, WHITELEDSTATE);   
    leds[briled].b = 0;                                   //these are all off
    leds[briled].g = 0;
    leds[briled].r = 0;
    leds[batled].b = 0;
    leds[batled].g = 0;
    leds[batled].r = 0;   
    FastLED.show();
}

// (the flash is a remnant of ledside without driver, so ... maybe there will be a flash driver, so its still here)
void FLASH(){
  uint8_t  FLASHLEDSTATE = PWMLIMIT;
  uint16_t  mv = readVcc();             
  analogWrite(WHITELED, FLASHLEDSTATE);
    leds[briled].b = 255 - FLASHLEDSTATE;
    leds[briled].g = 0;
    leds[briled].r = FLASHLEDSTATE;   
  FastLED.show();
  delay(2500);
  analogWrite(WHITELED, WHITELEDSTATE);
    leds[briled].b = 240 - WHITELEDSTATE;
    leds[briled].g = 0;
    leds[briled].r = WHITELEDSTATE;   
  FastLED.show();
}

// EOF  finally !
