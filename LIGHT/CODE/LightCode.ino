#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#include "QuadController.h"

#define LED 13
#define MAX_BRIGHT 200

enum SPEED{
  ZERO = 30,
  SLOW = 20,
  MEDIUM = 10,
  FAST = 2
};

enum BRIGHT{
  OFF = 0,
  TEN = 20,
  TWENTY = 40,
  THIRTY = 70,
  FOURTY = 120,
  EIGHTY = 200,
};

enum COLORS{
  RED,
  GREEN,
  BLUE
};

enum MODES{
  SOLID,
  SPARKLE
};

SPEED curSpeed = ZERO;
BRIGHT curBright = OFF;
COLORS curColor = RED;
MODES curState = SOLID;
byte tmpColor[3]={0,0,0};
Adafruit_NeoPixel strip = Adafruit_NeoPixel(18, 12,  NEO_GRB + NEO_KHZ800);
QuadController control = QuadController(14,15,18,19);
unsigned long timer = 0;

void setup() {
  // put your setup code here, to run once:  
  strip.begin();
  strip.setBrightness(curBright);
  strip.show(); // Initialize all pixels to 'off'
  pinMode(LED, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  unsigned long curTime = millis();
  control.update();

  updateStateMachine();
  updatePixels();
  
  if((millis()%1000)>900){
    digitalWrite(LED, HIGH);
  }
  else{
    digitalWrite(LED, LOW);
  }
}

void updatePixels(){

  switch(curState){
    case SPARKLE:
      tmpColor[0] = 3*sqrt(curBright);
      tmpColor[1] = 3*sqrt(curBright);
      tmpColor[2] = 3*sqrt(curBright);
      switch(curColor){
        case RED:
          tmpColor[0] = MAX_BRIGHT;
          break;
        case GREEN:
          tmpColor[1] = MAX_BRIGHT;
          break;
        case BLUE:
          tmpColor[2] = MAX_BRIGHT;
          break;
      }  
      if(millis()>timer){
        strip.setPixelColor(random(0,18), random(0,tmpColor[0]), random(0,tmpColor[1]), random(0,tmpColor[2]));
        timer = timer + ((curSpeed*curSpeed) * 5);
      }
      break;
    case SOLID:
      tmpColor[0] = 0;
      tmpColor[1] = 0;
      tmpColor[2] = 0;
      switch(curColor){
        case RED:
          tmpColor[0] = MAX_BRIGHT;
          break;
        case GREEN:
          tmpColor[1] = MAX_BRIGHT;
          break;
        case BLUE:
          tmpColor[2] = MAX_BRIGHT;
          break;
      }
      for(int i=0;i<18;i++){
        strip.setPixelColor(i, tmpColor[0],tmpColor[1], tmpColor[2]);
      }
      break;
  }
  strip.show();
  

}

void updateStateMachine(){
  
  button_state brightState = control.getButton(0);
  if(brightState==SHORT_PRESS){
    switch(curBright){
      case OFF:
        curBright = TEN;
        break;
      case TEN:
        curBright = TWENTY;
        break;
      case TWENTY:
        curBright = THIRTY;
        break;
      case THIRTY:
        curBright = FOURTY;
        break;
      case FOURTY:
        curBright = EIGHTY;
        break;
      case EIGHTY:
        curBright = OFF;
        break;
    }
    strip.setBrightness(curBright);
    strip.show();
  }
  
  button_state modeState = control.getButton(1);
  if(modeState==SHORT_PRESS){
    switch(curState){
      case SOLID:
        curState = SPARKLE;
        break;
      case SPARKLE:
        curState = SOLID;
        break;
    }
  }
  
  button_state colorState = control.getButton(2);
  if(colorState==SHORT_PRESS){
    switch(curColor){
      case RED:
        curColor = GREEN;
        break;
      case GREEN:
        curColor = BLUE;
        break;
      case BLUE:
        curColor = RED;
        break;
    }
    
  }
  
  button_state typeState = control.getButton(3);
  if(typeState==SHORT_PRESS){
    switch(curSpeed){
      case ZERO:
        curSpeed = SLOW;
        break;
      case SLOW:
        curSpeed = MEDIUM;
        break;
      case MEDIUM:
        curSpeed = FAST;
        break;
      case FAST:
        curSpeed = ZERO;
        break;
    }
    
  }
  
}

