#include "Adafruit_WS2801.h"
#include "SPI.h"

uint8_t dataPin  = 6;    // Grey wire on Adafruit Pixels
uint8_t clockPin = 9;    // Green wire on Adafruit Pixels

// Set the first variable to the NUMBER of pixels. 5 = 5 pixels in a row
Adafruit_WS2801 strip = Adafruit_WS2801(2, dataPin, clockPin);

int pinInter=2;

volatile int numFonction=0;
volatile boolean changeFonction=false;

int wait=20;

//déclaration fonction 
void off();
void White();
void France();
void Patriot();
void rainbow();
void rainbowCycle();

void compteurInter();


void (*Fonction[])(void)={off, White, France, Patriot, rainbow,rainbowCycle};
String nameFonction[]={"off","White","France","Patriot", "rainbow","rainbowCycle"};
volatile int sizeFonction=6;

void setup() {
  pinMode(pinInter,INPUT);
  digitalWrite(pinInter,HIGH);  //résitance de tirage
  attachInterrupt(digitalPinToInterrupt(pinInter), compteurInter, RISING);

  Serial.begin(9600);
  
  
  strip.begin();
  // Update LED contents, to start they are all 'off'
  Fonction[0]();
}

void loop() {
   (Fonction[numFonction])();
   if(changeFonction){
    noInterrupts();
    delay(1000); //stabilise l'entrée
      if(digitalRead(pinInter)==HIGH){
        numFonction++;
        
        if(numFonction>=sizeFonction)
          numFonction=0;   
        Serial.println(nameFonction[numFonction]);    
      }
      changeFonction=false;
      interrupts();
   }
   delay(100);
}

void compteurInter(){
  Serial.println("event");
  
  changeFonction=true;
}


/////////////////////////////////////
//  etat fonction
//////////////////////////////////////

void rainbow() {
  int i, j;
   
  for (j=0; j < 256; j++) {     // 3 cycles of all 256 colors in the wheel
    for (i=0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel( (i + j) % 255));
    }  
    strip.show();   // write all the pixels out
    if(changeFonction){      
      break;
    }
    delay(wait);
  }
}

// Slightly different, this one makes the rainbow wheel equally distributed 
// along the chain
void rainbowCycle() {
  int i, j;
  
  for (j=0; j < 256 * 5; j++) {     // 5 cycles of all 25 colors in the wheel
    for (i=0; i < strip.numPixels(); i++) {
      // tricky math! we use each pixel as a fraction of the full 96-color wheel
      // (thats the i / strip.numPixels() part)
      // Then add in j which makes the colors go around per pixel
      // the % 96 is to make the wheel cycle around
      strip.setPixelColor(i, Wheel( ((i * 256 / strip.numPixels()) + j) % 256) );
    }  
    strip.show();   // write all the pixels out
    if(changeFonction)
      break;
    delay(wait);
  }
}

void White(){
  colorWipe (Color(255,255,255),0);
}
void off(){  
  colorWipe (Color(0,0,0),0);
}

void Patriot(){
  colorWipe(Color(0,0,255),0);
  delay(500);
  colorWipe(Color(255,255,255),0);
  delay(500);
  colorWipe(Color(255,0,0),0);
  delay(400);
}

void France(){
  strip.setPixelColor(0, 0,0,255);
  strip.setPixelColor(1, 255,255,255);
  strip.setPixelColor(2, 255,255,255);
  strip.setPixelColor(3, 255,255,255);
  strip.setPixelColor(4, 255,0,0);
  strip.show();
}
/////////////////////////////////////
//  led fonction
//////////////////////////////////////

// fill the dots one after the other with said colorà
// good for testing purposes
void colorWipe(uint32_t c, uint8_t wait) {
  int i;
  
  for (i=0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}

// Create a 24 bit color value from R,G,B
uint32_t Color(byte r, byte g, byte b){
  uint32_t c;
  c = r;
  c <<= 8;
  c |= g;
  c <<= 8;
  c |= b;
  return c;
}

//Input a value 0 to 255 to get a color value.
//The colours are a transition r - g -b - back to r
uint32_t Wheel(byte WheelPos){
  if (WheelPos < 85) {
   return Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if (WheelPos < 170) {
   WheelPos -= 85;
   return Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170; 
   return Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}

