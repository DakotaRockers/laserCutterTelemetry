#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN            9
// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      10
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
unsigned long time0;
unsigned long time1;
unsigned long timeLimit = 7000;

int delayval = 50; // delay for half a second
float avg0;
float avg1;
bool isOpen0 = true;
bool isOpen1 = true;
bool lightOff0 = 0; 
bool lightOff1 = 0; //------------------
//static uint32_t lightOff1 = 0; //------------------

void setup() {
  pinMode(A0, INPUT_PULLUP);
  pinMode(A1, INPUT_PULLUP);  
  pinMode(A2, INPUT_PULLUP);  
  pinMode(2, INPUT_PULLUP);  
  pinMode(3, INPUT_PULLUP);  
  pinMode(4, INPUT_PULLUP);  
  pinMode(12, OUTPUT);
  Serial.begin(9600);
  Serial.println("Hello");
  strip.begin();
  strip.show();
}

void loop() {
  // Photocell 0 ---------------------
  //Serial.println(analogRead(A0));
  if (analogRead(A0)>=40){
    if ((millis()-time0) >= timeLimit){
      lightOff0 = 0;
    } 
  }
  else {
    lightOff0 = 1;
    time0 = millis();
  }
  // Photocell 1 ---------------------
  //Serial.println(analogRead(A1));
  if (analogRead(A1)>=350){
    if ((millis()-time1)>= timeLimit){
      lightOff1 = 0;
    } 
  }
  else {
    lightOff1 = 1;
    time1 = millis();
  }
  //Tilt Sensor 0 --------------
  int val0 = digitalRead(2)*100;
//  int val0 = analogRead(A1);
  avg0 = ((3.0*avg0)+val0)/4.0;

  if (avg0 > 50) {
    isOpen0 = false;
  }
  else {
    isOpen0 = true;
    //lightOff = 0;
  }
  
  //Tilt Sensor 1 -------------
  int val1 = digitalRead(3)*100;
//  int val1 = analogRead(A1);
  avg1 = ((3.0*avg1)+val1)/4.0;

  if (avg1 > 50) {
    isOpen1 = true;
  }
  else {
    isOpen1 = false;
    //lightOff = 0;
  }
  delay(90);
//  Serial.println(!isOpen1*8 + !lightOff1*4 + !isOpen0*2 + !lightOff0,BIN);
  Serial.write(!isOpen1*8 + !lightOff1*4 + !isOpen0*2 + !lightOff0);
  int Brightness = 128;
  int RED0 = !lightOff0 * Brightness;
  int GRN0 = !lightOff0 * !isOpen0 * Brightness;
  int BLU0 = lightOff0 * Brightness;
  int RED1 = !lightOff1 * Brightness;
  int GRN1 = !lightOff1 * !isOpen1 * Brightness;
  int BLU1 = lightOff1 * Brightness;
  colorWipe(strip.Color(RED0, GRN0, BLU0),strip.Color(RED1, GRN1, BLU1), 30);
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c,uint32_t d, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels()/2; i++) {
      strip.setPixelColor(i, c);
      strip.setPixelColor(i+5, d);
  }
  delay(wait);
  strip.show();
 }
