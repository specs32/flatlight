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

int LEDstate = 120;               // on poweron start main LED
int previousLEDstate = 120;       // set previous ledstate 
int lightMode = 3;                // set lightmode value

int ALTLEDstate = 0;              // on poweron start LED in lowest mode
int ALTpreviousLEDstate = 0;      // set previous ledstate 
int ALTlightMode = 1;             // set lightmode value

int steps = 0;                   // value for smooth fading

float mv = 0;                     // variable for calculating battery gauge
float power = 0;                  // variable for calculating duty cycle for the main LEDs


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
  // set the reference to Vcc and the measurement to the internal 1.2V-ish reference

  ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);

  delay(2);                           // Wait for Vref to settle
  ADCSRA |= _BV(ADSC);                // Start conversion
  while (bit_is_set(ADCSRA,ADSC));    // measuring

  uint8_t low  = ADCL;                // must read ADCL first - it then locks ADCH  
  uint8_t high = ADCH;                // unlocks both

  long result = (high<<8) | low;

  result = 1230000L / result;         // Calculate Vcc <--- ADJUST since it can be 10 % off (default 1.23V) 
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

 // monitor Battery
  mv = readVcc();
  while (mv < 2700){ // if Voltage is low low low ... blink red 
    
    digitalWrite(led,0); // cant give anymore
    digitalWrite(led2,0);

    digitalWrite(RED,1); // all but red
    digitalWrite(ORANGE,0);
    digitalWrite(YELLOW,0);
    digitalWrite(GREEN,0);
    digitalWrite(BRIGHTGREEN,0);
    
    delay(200);
    digitalWrite(RED,0); // which is blinking
    delay(200);
    digitalWrite(RED,1);
    delay(200);
    digitalWrite(RED,0);
    delay(800);
    //mv = readVcc();  that doesnt work here ... hmmm
    
  if (mv  > 3000){
    
    BATcheck();
    
    break;
    }
  }
  
  // display Battery gauge at startup
  if (millis()<1000){  
  analogWrite(led,0);
  analogWrite(led2,0);
  BATcheck();
  delay(2000);
  BATclear();
  analogWrite (led,LEDstate);
  analogWrite (led2,ALTLEDstate);
  }



// read and store temp 
  
  int val;                
  double temp;            
  val=analogRead(0);      
  temp=Thermistor(val); 


// if temp exceeds 50°C blink warning led and set LED output to lowest LEDstate, lets cool down a little
// i chose 50°C here because the NTC is not thearmaly coupled to an led, so it measures the PCB temp... LEDs will be much hotter !

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

      // if temperatur falls below 40°C go to 50% brightness

      if (temp < 40){
        LEDstate = 128;
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

  digitalWrite(led,0);
  digitalWrite(led2,0);
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
  analogWrite (led2,ALTLEDstate);

    hold = 0;
    }   
  }
  
  lastReading = reading;
  
  if (single == 1 && (millis() - lastSwitchTime) > doubleTime) {

// button single click (set brightness WHITE)

    LED();
    single = 0;
  }
}


// button routine (what happened?)

void onRelease() {

// was it not a single click ?

  if ((millis() - lastSwitchTime) >= doubleTime) {
    single = 1;
    lastSwitchTime = millis();
    return;
  }  

// was it a double click ?

  if ((millis() - lastSwitchTime) < doubleTime) {
      ALTLED();          
      single = 0;
      lastSwitchTime = millis();
  }  
}

// Main LEDs
void LED() {

  mv = readVcc();
  power = 2700 / mv;

  lightMode = lightMode + 1;

    if (lightMode == 5){
      lightMode = 1;
      }
    
    if (lightMode == 1){
      LEDstate = 0;
      fade();
      previousLEDstate = LEDstate;
    
    } else if (lightMode == 2) {
      LEDstate = 8;
      fade();
      previousLEDstate = LEDstate;
    
    } else if (lightMode == 3) {
      LEDstate = (128 * power);
      fade();
      previousLEDstate = LEDstate;
    }
    
    else if (lightMode == 4){
      LEDstate = (255 * power); 
      fade();
      previousLEDstate = LEDstate;
    }
}


// Alternative LEDs
void ALTLED() {
  
  
  ALTlightMode = ALTlightMode + 1;

    if (ALTlightMode == 5)
    {
    ALTlightMode = 1;
    }
    if (ALTlightMode == 1)
    {
    ALTLEDstate = 0;
    ALTfade();
    ALTpreviousLEDstate = ALTLEDstate;
    } else if (ALTlightMode == 2) {
    ALTLEDstate = 2;
    ALTfade();
    ALTpreviousLEDstate = ALTLEDstate;
    } else if (ALTlightMode == 3) {
    ALTLEDstate = (128 * power);
    ALTfade();
    ALTpreviousLEDstate = ALTLEDstate;
    }
    else if (ALTlightMode == 4)
    {
    ALTLEDstate = (255 * power);
    ALTfade();
    ALTpreviousLEDstate = ALTLEDstate;
    }
   
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


void ALTfade(){

  steps = 64 / ALTLEDstate;

    // fade up
    
    for (int i=ALTpreviousLEDstate; i <=ALTLEDstate; i++){
        analogWrite(led2, i);
        delay(steps);
    }

    // fade down a bit faster
    
    for (int i=ALTpreviousLEDstate; i >=ALTLEDstate; i--){
        analogWrite(led2, i);
        delay(steps/8);
    }
    
}


// battery check routine
void BATcheck(){
   mv = 0;
   mv = readVcc();

    if (mv < 3200) {
    digitalWrite (RED,1);
    digitalWrite (ORANGE,0);
    digitalWrite (YELLOW,0);
    digitalWrite (GREEN,0);
    digitalWrite (BRIGHTGREEN,0);
    } 

  if (mv > 3200 && mv < 3400) {
    digitalWrite (RED,1);
    digitalWrite (ORANGE,1);
    digitalWrite (YELLOW,0);
    digitalWrite (GREEN,0);
    digitalWrite (BRIGHTGREEN,0);
    } 

  if (mv > 3400 && mv < 3600) {
    digitalWrite (RED,1);
    digitalWrite (ORANGE,1);
    digitalWrite (YELLOW,1);
    digitalWrite (GREEN,0);
    digitalWrite (BRIGHTGREEN,0);
    } 

  if (mv > 3600 && mv < 4000) {
    digitalWrite (RED,1);
    digitalWrite (ORANGE,1);
    digitalWrite (YELLOW,1);
    digitalWrite (GREEN,1);
    digitalWrite (BRIGHTGREEN,0);
    } 

  if (mv > 4000) {
    digitalWrite (RED,1);
    digitalWrite (ORANGE,1);
    digitalWrite (YELLOW,1);
    digitalWrite (GREEN,1);
    digitalWrite (BRIGHTGREEN,1);
    } 
  delay(2);
  }

void BATclear(){

    digitalWrite (RED,0);
    digitalWrite (ORANGE,0);
    digitalWrite (YELLOW,0);
    digitalWrite (GREEN,0);
    digitalWrite (BRIGHTGREEN,0);
  
  
  }

// loop ends


