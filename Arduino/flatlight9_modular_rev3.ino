/* 

   This is the Code for the modular Flatlight (PINOUT)

    
     
    Copyright (C) <2017>  <Nicolas Rosenthal> nicoandmomo  gmail  com

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/



// define PINS

const int button = 0;           // define button pin
const int led = 10;             // define LED pin
const int led2 = 9;             // define LED pin red
const int templed = 1;          // define templed pin

const int RED = 15;               // Battery Indicator Pin
const int ORANGE = 16;            // Battery Indicator Pin
const int YELLOW = 17;            // Battery Indicator Pin
const int GREEN = 18;             // Battery Indicator Pin
const int BRIGHTGREEN = 19;       // Battery Indicator Pin


int bounceTime = 50;              // define button debounce
int holdTime = 250;               // define button hold time
int doubleTime = 500;             // define button doubleclick time

int lastReading = LOW;            // button read value
int hold = 0;                     // button hold value
int single = 0;                   // button single press value
long onTime = 0;                  // button read ontime
long lastSwitchTime = 0;          // button switchtime value

int LEDstate = 120;               // on poweron start LED in lowest mode
int previousLEDstate = 120;       // set previous ledstate 
int lightMode = 3;                // set lightmode value

int REDLEDstate = 0;              // on poweron start LED in lowest mode
int REDpreviousLEDstate = 0;      // set previous ledstate 
int REDlightMode = 1;             // set lightmode value

long steps = 0;                   // value for smooth fading


// calculate temp in °C from ntc (just to make the values clearer(for me))

double Thermistor(int RawADC) {
  double Temp;
  Temp = log(10000.0*((1024.0/RawADC-1))); 
  Temp = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * Temp * Temp ))* Temp );
  Temp = Temp - 273.15;          
  return Temp;
  }

//  check supply voltage

long readVcc() {
  
  // Read reference against AVcc
  // set the reference to Vcc and the measurement to the internal 1.2V-ish reference, man .. 10 % .. really ?

  ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);

  delay(2);                           // Wait for Vref to settle
  ADCSRA |= _BV(ADSC);                // Start conversion
  while (bit_is_set(ADCSRA,ADSC));    // measuring

  uint8_t low  = ADCL;                // must read ADCL first - it then locks ADCH  
  uint8_t high = ADCH;                // unlocks both

  long result = (high<<8) | low;

  result = 1315300L / result;         // Calculate Vcc <--- ADJUST since it can be 10 % off
  return result;                      // Vcc in millivolts
}


void setup() {

  // make PWM Fast(er) (1/64 CPU CLK ~ 125 KHz)
  
  TCCR1A = (1<<WGM10)|(1<<COM1A1)|(1<<COM1B1);
  TCCR1B = (1<<WGM12)|(1<<CS10)|(1<<CS11);     

  
  //define PIN Modes  
  
  pinMode(button, INPUT);
  pinMode(led, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(templed, OUTPUT);
  pinMode(RED, OUTPUT);
  pinMode(ORANGE, OUTPUT);
  pinMode(YELLOW, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BRIGHTGREEN, OUTPUT);

}

// start main loop 

void loop() {


  // check Battery at startup
  if (millis()<2000){  
  analogWrite(led,0);
  analogWrite(led2,0);
  BATcheck();
  delay(2);
  BATclear();
  analogWrite (led,LEDstate);
  analogWrite (led2,REDLEDstate);
  }



// read and store temp 
  
  int val;                
  double temp;            
  val=analogRead(0);      
  temp=Thermistor(val); 


// if temp exceeds 50°C blink warning led and set LED output to lowest LEDstate, lets cool down a little
// i chose 50°C here because the NTC is not thearmaly coupled to an led, so it measures the PCB temp... LEDs will be much hotter !
// play it safe... or put a higher value and toast your lipo 

while(temp > 50){
    
    LEDstate = 8;
    previousLEDstate = LEDstate;
    fade ();

    digitalWrite(templed, 1);
    delay(50);
    digitalWrite(templed, 0);
    delay(50);

    // double check ;)

    int val;                
    double temp;            
    val=analogRead(0);      
    temp=Thermistor(val); 

      // if temperatur falls blow 35°C restore normal operation

      if (temp < 35){
        LEDstate = 32;
        previousLEDstate = LEDstate;
        fade ();
        break;
      }
}

// read button state and do something ;)

int reading = digitalRead(button);

//button first pressed
  if (reading == HIGH && lastReading == LOW) {
    onTime = millis();
  }

//button held (as long as held switch all other LEDs off to get a real VCC value to read out.. no drag)
  if (reading == HIGH && lastReading == HIGH) {
    if ((millis() - onTime) > holdTime) {

  analogWrite(led,0);
  analogWrite(led2,0);
  BATcheck ();

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

  BATclear();
  analogWrite (led,LEDstate);
  analogWrite (led2,REDLEDstate);

    hold = 0;
    }   
  }
  
  lastReading = reading;
  
  if (single == 1 && (millis() - lastSwitchTime) > doubleTime) {

// button single click (set brightness WHITE)

  lightMode = lightMode + 1;

    if (lightMode == 5)
    {
    lightMode = 1;
    }
    if (lightMode == 1)
    {
    LEDstate = 0;
    fade();
    previousLEDstate = LEDstate;
    } else if (lightMode == 2) {
    LEDstate = 8;
    fade();
    previousLEDstate = LEDstate;
    } else if (lightMode == 3) {
    LEDstate = 127;
    fade();
    previousLEDstate = LEDstate;
    }
    else if (lightMode == 4)
    {
    LEDstate = 234;
    fade();
    previousLEDstate = LEDstate;
    }

    single = 0;
  }
}


// button routine (what happened?)

void onRelease() {

// was it a single click ?

  if ((millis() - lastSwitchTime) >= doubleTime) {
    single = 1;
    lastSwitchTime = millis();
    return;
  }  

// was it a double click ?

  if ((millis() - lastSwitchTime) < doubleTime) {
      redLED();          
      single = 0;
      lastSwitchTime = millis();
  }  
}

void redLED() {
  
  
  REDlightMode = REDlightMode + 1;

    if (REDlightMode == 5)
    {
    REDlightMode = 1;
    }
    if (REDlightMode == 1)
    {
    REDLEDstate = 0;
    REDfade();
    REDpreviousLEDstate = REDLEDstate;
    } else if (REDlightMode == 2) {
    REDLEDstate = 2;
    REDfade();
    REDpreviousLEDstate = REDLEDstate;
    } else if (REDlightMode == 3) {
    REDLEDstate = 16;
    REDfade();
    REDpreviousLEDstate = REDLEDstate;
    }
    else if (REDlightMode == 4)
    {
    REDLEDstate = 64;
    REDfade();
    REDpreviousLEDstate = REDLEDstate;
    }
   
} 
  

// return LED to previous state

void prevLED(){
  analogWrite(led,LEDstate);
}
  

// smootly fade LED to state

void fade(){
  
  steps = 128 / LEDstate;

    // fade up
    
    for (int i=previousLEDstate; i <=LEDstate; i++){
        analogWrite(led, i);
        delay(steps);
    }

    // fade down a bit faster
    
    for (int i=previousLEDstate; i >=LEDstate; i--){
        analogWrite(led, i);
        delay(steps/8);
    }
    
}


void REDfade(){

  steps = 64 / REDLEDstate;

    // fade up
    
    for (int i=REDpreviousLEDstate; i <=REDLEDstate; i++){
        analogWrite(led2, i);
        delay(steps);
    }

    // fade down a bit faster
    
    for (int i=REDpreviousLEDstate; i >=REDLEDstate; i--){
        analogWrite(led2, i);
        delay(steps/8);
    }
    
}


// battery check routine
void BATcheck(){
  
  float mv = readVcc();

    if (mv > 3300 && mv < 3500) {
    digitalWrite (RED,1);
    digitalWrite (ORANGE,0);
    digitalWrite (YELLOW,0);
    digitalWrite (GREEN,0);
    digitalWrite (BRIGHTGREEN,0);
    } 

  if (mv > 3500 && mv < 3700) {
    digitalWrite (RED,1);
    digitalWrite (ORANGE,1);
    digitalWrite (YELLOW,0);
    digitalWrite (GREEN,0);
    digitalWrite (BRIGHTGREEN,0);
    } 

  if (mv > 3700 && mv < 3900) {
    digitalWrite (RED,1);
    digitalWrite (ORANGE,1);
    digitalWrite (YELLOW,1);
    digitalWrite (GREEN,0);
    digitalWrite (BRIGHTGREEN,0);
    } 

  if (mv > 3900 && mv < 4100) {
    digitalWrite (RED,1);
    digitalWrite (ORANGE,1);
    digitalWrite (YELLOW,1);
    digitalWrite (GREEN,1);
    digitalWrite (BRIGHTGREEN,0);
    } 

  if (mv > 4100) {
    digitalWrite (RED,1);
    digitalWrite (ORANGE,1);
    digitalWrite (YELLOW,1);
    digitalWrite (GREEN,1);
    digitalWrite (BRIGHTGREEN,1);
    } 
  delay(2000);
  }

void BATclear(){

    digitalWrite (RED,0);
    digitalWrite (ORANGE,0);
    digitalWrite (YELLOW,0);
    digitalWrite (GREEN,0);
    digitalWrite (BRIGHTGREEN,0);
  
  
  }

// loop ends


