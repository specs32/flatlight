/* flatlight revision 18
 *  by http://github.com/specs32 (sorry !)
 *  atmega 328 - 8 MHZ intern OSC -> avrdude -e -c [PROGRAMMER] -pm328 -B10 -U lfuse:w:0xe2:m -U hfuse:w:0xd9:m -U efuse:w:0xff:m
 *  2 x APA102 - 2020  status-leds
 *  2 Buttons
 *  1 IMU
 *  8 Cree J-Series HighPower
 *  4 Cree XP-E
 *
 */


// INDICATORS
#include <FastLED.h>                                            // please check out https://github.com/FastLED/FastLED for license and such
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

// TIMERS
#include <SimpleTimer.h>                                      // out of the arduino repo
SimpleTimer TEMPCHECK;
SimpleTimer BATCHECK;
SimpleTimer PWMCHECK;

// IMU
#include "SparkFunLSM6DS3.h"                                  // please check out https://github.com/sparkfun/SparkFun_LSM6DS3_Arduino_Library for license and such
LSM6DS3 myIMU( I2C_MODE, 0x6A );                              //I2C device address 0x6A

// INT 1
const int int1Pin = 2;                                        //Use pin 2 for int.0 on uno
uint8_t int1Status = 0;
uint8_t t_toggle = 0;                                         // tap toggle
uint8_t t_toggle_last = 0;
uint8_t dt_toggle = 0;                                        // dt toggle
uint8_t dt_toggle_last = 0;

// INT 2
const int int2Pin = 3;                                        //Use pin 2 for int.0 on uno
uint8_t int2Status = 0;
uint8_t wu_toggle = 0;

// BUTTON PINS
const int BUTTON_UP = 17;                                    // define the up button pin
const int BUTTON_DOWN = 16;                                  // define the down button pin

// LED PINS
const int LIGHT = 10;                                         // PowerLED PWM control. (OC1B // PB2 // PIN10)
const int BEAM = 9;                                           // PowerLED  PWM control. (OC1A // PB1 // PIN9)

// CONSTRAINS to the leds
uint8_t LIGHTPWMLIMIT = 150;                                  // PWMLIMIT FOR THE FULLY CHARGED LIPO !!!! 
uint8_t BEAMPWMLIMIT = 150;                                   // PWMLIMIT FOR THE FULLY CHARGED LIPO !!!! 
uint16_t LIGHTMAXTEMP = 40;                                   // (when to STEP down !) Depends very strong on the thermal connection between LED and NTC... 40 works for me here atm.
uint16_t BEAMMAXTEMP = 45;                                    // these leds have a heatsink pad coupled to the ntc (on ledside no. 2), temps will read higher !
uint8_t LIGHTCRITTEMP = 60;                                   // (when to SHUT down  !) compensate for overshoot ...
uint8_t BEAMCRITTEMP = 60;
uint16_t Vf_BEAM = 2800;                                      // led max. forward voltage
uint16_t Vf_LIGHT = 3000;    
uint8_t LIGHTSTATE = 0;                                       // LIGHT @ 1-(PWMLIMIT)
uint8_t BEAMSTATE = 0;               

// LIPO Setup
uint16_t bat_full = 4385;                                     // Lipo charged
uint16_t bat_empty = 2750;                                    // Lipo discharged 

// BUTTON UP
boolean up_lastReading = HIGH;                                // button read value
long up_onTime = 0;                                           // button read ontime
long up_lastSwitchTime = 0;                                   // button switchtime value
uint8_t up_hold = 0;                                          // button hold value
uint8_t up_single = 0;                                        // button single press value

// BUTTON DOWN
boolean down_lastReading = HIGH;                              // button read value
long down_onTime = 0;                                         // button read ontime
long down_lastSwitchTime = 0;                                 // button switchtime value
uint8_t down_hold = 0;                                        // button hold value
uint8_t down_single = 0;

// BUTTON SETTINGS
const uint8_t bounceTime = 50;                                // define button debounce
const uint8_t holdTime = 250;                                 // define button hold time
const uint16_t doubleTime = 700;                              // define button doubleclick time

// PLASMA FUNCTION
#define qsubd(x, b)  ((x>b)?b:0)                              // Digital unsigned subtraction macro. if result <0, then => 0. Otherwise, take on fixed value.
#define qsuba(x, b)  ((x>b)?x-b:0)                            // Analog Unsigned subtraction macro. if result <0, then => 0

// SETUP SETUP SETUP SETUP SETUP SETUP SETUP SETUP SETUP SETUP SETUP SETUP SETUP SETUP SÄÄÄTUUUUUUUPPPPP
void setup() {
  
  // grace
  delay(500);
  
  // debug
  Serial.begin(9600);

  // setup pixels
  LEDS.addLeds<LED_TYPE, LED_DT, LED_CK, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(max_bright);
  currentPalette = OceanColors_p;

  // timers
  TEMPCHECK.setInterval(53, CHECKTEMP);                       // call  every x milliseconds
  BATCHECK.setInterval(3001, BATDISP);               
  PWMCHECK.setInterval(557, SETPWM);               

  // IMU SETUP
  myIMU.begin();                                                        // This is the config for a LSM6DSM ! (check datasheet which registers to pull !)
  myIMU.writeRegister( LSM6DS3_ACC_GYRO_CTRL1_XL, 0x61);
  myIMU.writeRegister( LSM6DS3_ACC_GYRO_CTRL8_XL, 0x04);
  myIMU.writeRegister( LSM6DS3_ACC_GYRO_TAP_CFG1, 0x9E );               
  myIMU.writeRegister( LSM6DS3_ACC_GYRO_TAP_THS_6D, 0x8C );
  myIMU.writeRegister( LSM6DS3_ACC_GYRO_INT_DUR2, 0x7F );              
  myIMU.writeRegister( LSM6DS3_ACC_GYRO_WAKE_UP_THS, 0x81 );            // threshold settings for taps and wakeup
  myIMU.writeRegister( LSM6DS3_ACC_GYRO_WAKE_UP_DUR, 0x0F );            // threshold settings for sleep and wakeup 
  myIMU.writeRegister( LSM6DS3_ACC_GYRO_MD1_CFG, 0x48 );                // interrupt 1 event config (tap and wakeup) // 48 for tap
  myIMU.writeRegister( LSM6DS3_ACC_GYRO_MD2_CFG, 0x20 );                // interrupt 2 event config (inactivity and wakeup)

  // setting up the interrupt pins
  pinMode(int1Pin, INPUT);                                       
  attachInterrupt(digitalPinToInterrupt(int1Pin), int1ISR, RISING); 
  pinMode(int2Pin, INPUT);                                      
  attachInterrupt(digitalPinToInterrupt(int2Pin), int2ISR, RISING);

  //define BUTTON AND LIGHT PIN Modes
  pinMode(BUTTON_UP, INPUT_PULLUP);
  pinMode(BUTTON_DOWN, INPUT_PULLUP);
  pinMode(LIGHT, OUTPUT);
  pinMode(BEAM, OUTPUT);

  // FastPWM 9-bit (something is there about it ... hmmm cap on the fet ?)
  TCCR1A =  _BV(COM1A1) | _BV(COM1B1) | _BV(WGM11); // _BV(WGM10) |
  TCCR1B = _BV(CS10) | _BV(WGM12);

} // setup end

// LOOP LOOP LOOP LOOP LOOP LOOP LOOP LOOP LOOP LOOP LOOP LOOP LOOP LOOP LOOP OF INFINITY
void loop() {
  
  // TIMERS
  TEMPCHECK.run();
  BATCHECK.run();
  PWMCHECK.run();

  // INT1 TAP
  if( int1Status > 0 ) {
      delay(300);
      // TAP
      if( int1Status == 1 ) {
//        Serial.println("T");
        if (t_toggle == 0){
        t_toggle = 1;
        } else if (t_toggle =! 0){
        t_toggle = 0;
        }
      }
      // DOuBLE TAP
      if( int1Status > 1 )  {
//        Serial.println("DT");
        if (dt_toggle == 0){
        dt_toggle = 1;
        } else if (dt_toggle =! 0){
        dt_toggle = 0;
        dt_toggle_last = 1;
        }
      }
      int1Status = 0;
  }

  // INT2 WAKEUP
  
//  if(digitalRead(int2Pin) == LOW) {
//    //Serial.println("SLEEP");
//    
//    while(true){
//      EVERY_N_MILLISECONDS(25) {                                  // FastLED based non-blocking delay to update/display the sequence.
//        plasma();
//        }
//      EVERY_N_MILLISECONDS(500) {
//        uint8_t maxChanges = 56; 
//        nblendPaletteTowardPalette(currentPalette, targetPalette, maxChanges);   // AWESOME palette blending capability.
//        }
//      EVERY_N_SECONDS(50) {                                 // Change the target palette to a random one every 5 seconds.
//        uint8_t baseC = random8();                         // You can use this as a baseline colour if you want similar hues in the next line.
//        targetPalette = CRGBPalette16(CHSV(baseC+random8(32), 192, random8(128,255)), CHSV(baseC+random8(32), 255, random8(128,255)), CHSV(baseC+random8(32), 192, random8(128,255)), CHSV(baseC+random8(32), 255, random8(128,255)));
//        }
//      FastLED.show();
//    if (digitalRead(int2Pin) == HIGH){
//      break;
//      }
//    }
//    int2Status = 0;
//  }
 
  // Button UP 
  int up_reading = digitalRead(BUTTON_UP);
  if (up_reading == LOW && up_lastReading == HIGH) {
    up_onTime = millis();
  }
  if (up_reading == LOW && up_lastReading == LOW) {
    if ((millis() - up_onTime) > holdTime) {
      // do something when held
      LIGHTSTATE_UP();                                 
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
     // button single click
     LIGHTSTATE_UP();                                    
    up_single = 0;
  }

  // Button DOWN 
  int down_reading = digitalRead(BUTTON_DOWN);
  if (down_reading == LOW && down_lastReading == HIGH) {
    down_onTime = millis();
  }
  if (down_reading == LOW && down_lastReading == LOW) {
    if ((millis() - down_onTime) > holdTime) {
      // do something on hold
      LIGHTSTATE_DOWN();                                     
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
    // button single click
    LIGHTSTATE_DOWN();
    down_single = 0;
  }
} // loop end

void SLEEP(){
  wu_toggle = 0;
  // Power saving
  //    PRR =  _BV(PRUSART0) | _BV(PRTWI) | _BV(PRSPI); //_BV(PRTIM2) | _BV(PRTIM0) |
  //    MCUCR = _BV(BODSE) | _BV(BODS);
  while(true){
    
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
        break;
      }
  }
      
}




// NTC Sensor dependent LIGHTLEVEL calculation bla
void CHECKTEMP(){ 
  // check for crtitcal temps first !
  // BEAM
  if (avgNTC0() > BEAMCRITTEMP){
    BEAMSTATE = 0;
    analogWrite(BEAM, BEAMSTATE);
    }
  // LIGHT
  if (avgNTC1() > LIGHTCRITTEMP){
    LIGHTSTATE = 0;
    analogWrite(LIGHT, LIGHTSTATE);
    }

  // always down
  if (avgNTC0() > BEAMMAXTEMP && BEAMSTATE > 2){
    BEAMSTATE = BEAMSTATE - 2;
    analogWrite(BEAM, BEAMSTATE);
  }
  if (avgNTC1() > LIGHTMAXTEMP && LIGHTSTATE > 2){
    LIGHTSTATE = LIGHTSTATE - 2;
    analogWrite(LIGHT, LIGHTSTATE);
  }

  // CLEAR ?
  if (dt_toggle_last == 1 && dt_toggle == 0){
      BEAMSTATE = 0;
      analogWrite(BEAM, BEAMSTATE);
      LIGHTSTATE = 0;
      analogWrite(LIGHT, LIGHTSTATE);
      dt_toggle_last = 0;
  }

  // AUTO ?
  if (dt_toggle == 1){
    // LIGHTS UP
    if (avgNTC1() < LIGHTMAXTEMP && LIGHTSTATE < LIGHTPWMLIMIT) {
        LIGHTSTATE = LIGHTSTATE + 1;
        analogWrite(LIGHT, LIGHTSTATE);
      }
    
    // BEAM ON ?
    if (t_toggle == 1){
      if (t_toggle_last == 1){
        if (avgNTC0() < BEAMMAXTEMP && BEAMSTATE < BEAMPWMLIMIT) {
          BEAMSTATE = BEAMPWMLIMIT;
          analogWrite(BEAM, BEAMSTATE);
          t_toggle_last = 0;
        }
      }

      if (avgNTC0() < BEAMMAXTEMP && BEAMSTATE < BEAMPWMLIMIT) {
          BEAMSTATE = BEAMSTATE + 1;
          analogWrite(BEAM, BEAMSTATE);
        }
      if (LIGHTSTATE > 1){
          LIGHTSTATE = 0;
          analogWrite(LIGHT, LIGHTSTATE);
      } 
      
    }
    // BEAM OFF ?
    if (t_toggle == 0){
      if (t_toggle_last == 0){
        if (avgNTC1() < LIGHTMAXTEMP && LIGHTSTATE < LIGHTPWMLIMIT) {
          LIGHTSTATE = LIGHTPWMLIMIT;
          analogWrite(LIGHT, LIGHTSTATE);
          t_toggle_last = 1;
        }
      }

      if (avgNTC1() < LIGHTMAXTEMP && LIGHTSTATE < LIGHTPWMLIMIT) {
          LIGHTSTATE = LIGHTSTATE + 1;
          analogWrite(LIGHT, LIGHTSTATE);
        }
      if (BEAMSTATE > 1){
        BEAMSTATE = 0;
        analogWrite(BEAM, BEAMSTATE);
      }
      
    }
  }
  
  // write lightstate
  APA_bri(BEAMSTATE);
}

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
    FLASH();                  // do something on double click
    up_single = 0;
    up_lastSwitchTime = millis();
  }
}

// Brightness routines //
void LIGHTSTATE_UP() {

  if (avgNTC0() > BEAMMAXTEMP | avgNTC1() > LIGHTMAXTEMP ){
    LIGHTSTATE = 10;
    BEAMSTATE = 0;
    analogWrite(LIGHT, LIGHTSTATE);
    analogWrite(BEAM, BEAMSTATE);
    for (int o = 1; o < 20; o++){
      APA_bri(110);
      delay(300);
      APA_bri(255);
      delay(100);
  
    }
  } else {
    while(true){
    if (LIGHTSTATE >= LIGHTPWMLIMIT-5) {
      LIGHTSTATE = LIGHTPWMLIMIT;
      analogWrite(LIGHT, LIGHTSTATE);
      APA_bri(LIGHTSTATE);
      delay(30);
    } else {
      LIGHTSTATE = LIGHTSTATE + 5;
      analogWrite(LIGHT, LIGHTSTATE);
      APA_bri(LIGHTSTATE);
      delay(30);
    }
    int kitkat = digitalRead(BUTTON_UP);
      if (kitkat == HIGH) {
        break;
      }
    }
  }
}

void LIGHTSTATE_DOWN() {
  while(true){
  if (LIGHTSTATE <= 5) {
    LIGHTSTATE = 0;
    analogWrite(LIGHT, LIGHTSTATE);
    APA_bri(LIGHTSTATE);
    delay(30);
  } else {
    LIGHTSTATE = LIGHTSTATE - 5;
    analogWrite(LIGHT, LIGHTSTATE);
    APA_bri(LIGHTSTATE);
    delay(30);
  }
  int kitkat = digitalRead(BUTTON_DOWN);
    if (kitkat == HIGH) {
      break;
    }
  }
}



// BATTERY CALCULATIONS AND SETTINGS
void BATDISP(){

  uint16_t bv = batVCC();                           // read Voltage 
//    Serial.print("bv = ");
//    Serial.println(bv);
  uint16_t dv = bat_full - bat_empty;               // charge range
  uint16_t uv = bv - bat_empty;                     // volts "left"
  float batpercent = (float) uv / dv * 100;         // percent of full charge 
  uint8_t temp = ((int)batpercent * 255)/100;       // make it a value between 0 and 255

  if (bv >= bat_full - 200){
    leds[batled].b = 100;
    leds[batled].g = 230;
    leds[batled].r = 0;
    FastLED.show();
  } else {
    leds[batled].b = 0;
    leds[batled].g = temp;                          // feed it to the display
    leds[batled].r = 255 - temp ;
    FastLED.show();
  }
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


// Enough to read and last as long as possible
void READBOOK() {
    LIGHTSTATE = 1;                                     // 1 might be too low 
    BEAMSTATE = 0;                                      // no beam
    analogWrite(LIGHT, LIGHTSTATE);
    analogWrite(BEAM, BEAMSTATE);
    leds[briled].b = 0;                                   //these are all off to save energy
    leds[briled].g = 0;                                   // could put plasma here
    leds[briled].r = 0;
    leds[batled].b = 0;
    leds[batled].g = 0;
    leds[batled].r = 0;
    FastLED.show();
}

// unused ... maybe when i finally think of fitting caps parallel to the mosfret (probably need differend mosfets then)
void FLASH(){
  analogWrite(BEAM, BEAMPWMLIMIT);
  delay(100);
  analogWrite(BEAM, BEAMSTATE);
}

//  check supply voltage
long readVcc() {
  ADMUX = 0b01001110;//_BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);   // (m328)
  delay(4);                           // Wait for Vref
  ADCSRA |= _BV(ADSC);                // Start conversion
  while (bit_is_set(ADCSRA, ADSC));   // measuring
  uint8_t low  = ADCL;                // must read ADCL first - it then locks ADCH
  uint8_t high = ADCH;                // unlocks both
  long result = (high << 8) | low;
  result = 1140000L / result;         // Calculate Vcc, Bandgap +/- 10 % -- ADJUST !!
  return result;                      // Vcc in millivolts
}

uint16_t batVCC(void) {
    uint16_t imax=0;
    uint16_t imin=65535;
    uint16_t isum=0;
    uint16_t itemp=0;
    uint8_t ctr;
    for (ctr=10;ctr>0;ctr--) {
        itemp=readVcc();
        if (itemp<imin) imin=itemp;
        if (itemp>imax) imax=itemp;
        isum+=itemp;
        delay(4);
    }
    isum-=imin;
    isum-=imax; //subtract the min and max from the sum, now only 8 are left
    itemp=isum/8; //compiler will automatically optimize this to a shift
    return itemp;
}


uint8_t APA_bri(int BRI_setting){

      leds[briled].b = (255 - BRI_setting);
      leds[briled].g = 0;
      leds[briled].r = BRI_setting;
      FastLED.show();
  }

// calculate temp in °C from ntc analog raw data
double Thermistor(int RawADC) {
  double Temp;
  Temp = log(10000.0 * ((1024.0 / RawADC - 1)));
  Temp = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * Temp * Temp )) * Temp );
  Temp = Temp - 273.15;
  return Temp;
}

uint16_t avgNTC0(void) {
    ADMUX = 0b01000000;
    int val;
    uint16_t imax=0;
    uint16_t imin=65535;
    uint16_t isum=0;
    uint16_t itemp=0;
    uint8_t ctr;
    for (ctr=10;ctr>0;ctr--) {
        val=analogRead(0);
        itemp=Thermistor(val);
        if (itemp<imin) imin=itemp;
        if (itemp>imax) imax=itemp;
        isum+=itemp;
        delay(4);
    }
    isum-=imin;
    isum-=imax; //subtract the min and max from the sum, now only 8 are left
    itemp=isum/8; //compiler will automatically optimize this to a shift
    return itemp;
}

uint16_t avgNTC1(void) {
    ADMUX = 0b01000001;
    int val;
    uint16_t imax=0;
    uint16_t imin=65535;
    uint16_t isum=0;
    uint16_t itemp=0;
    uint8_t ctr;
    for (ctr=10;ctr>0;ctr--) {
        val=analogRead(1);
        itemp=Thermistor(val);
        if (itemp<imin) imin=itemp;
        if (itemp>imax) imax=itemp;
        isum+=itemp;
        delay(4);
    }
    isum-=imin;
    isum-=imax; //subtract the min and max from the sum, now only 8 are left
    itemp=isum/8; //compiler will automatically optimize this to a shift
    return itemp;
}


// SET PWMLIMIT (based on voltage to meet vf_LED/BEAM
void SETPWM(){
 
  uint16_t bv = batVCC();  // read Voltage 
  uint16_t dv = bat_full - bat_empty; // charge range

  // BEAM
  uint16_t vbeam;
  if (bat_full > Vf_BEAM){
    vbeam = Vf_BEAM + dv - bv   ;
  } else { 
    vbeam = Vf_BEAM;
  }
  float vbpercent = (float) vbeam / dv * 100;
  uint8_t vbtemp = ((int)vbpercent * 255)/100;
  BEAMPWMLIMIT = vbtemp ;

  // LIGHT
  uint16_t vlight;
  if (bat_full > Vf_LIGHT){
    vlight = Vf_LIGHT + dv - bv   ;
  } else { 
    vlight = Vf_LIGHT;
  }
  float vlpercent = (float) vlight / dv * 100;
  uint8_t vltemp = ((int)vlpercent * 255)/100;
  LIGHTPWMLIMIT = vltemp ;
}

void int1ISR()
{
  int1Status++;
}

void int2ISR()
{
  int2Status++;
}
// EOF
