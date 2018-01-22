#include <arduino.h>
#include <Led.h>
#include <LedList.h>

int OneShortBlinks[20] = { 800, 200};
int TwoShortBlinks[20] = { 600, 200, 200, 200};
int ThreeShortBlinks[20] = { 600, 200, 200, 200, 200, 200};

int OneLongBlink[20] = { 500, 500};


int Xyz_Blinks_0[20] = {};
int Xyz_Blinks_1[20] = { 800, 200};
int Xyz_Blinks_2[20] = { 800, 200, 200, 200};
int Xyz_Blinks_3[20] = { 800, 200, 200, 200, 200, 200};
int Xyz_Blinks_12[20] = { 800, 200,     800, 200, 200, 200};
int Xyz_Blinks_13[20] = { 800, 200,     800, 200, 200, 200, 200, 200};
int Xyz_Blinks_23[20] = { 800, 200, 200, 200,     800, 200, 200, 200, 200, 200};
int Xyz_Blinks_123[20] = { 800, 200,     800, 200, 200, 200,     800, 200, 200, 200, 200, 200};

int *Blinks[8] = {  Xyz_Blinks_0,  Xyz_Blinks_1,  Xyz_Blinks_2,  Xyz_Blinks_12,
                  Xyz_Blinks_3,  Xyz_Blinks_13, Xyz_Blinks_23, Xyz_Blinks_123 };;


LedList Led::ledList = LedList();

Led::Led( int p){
  pin =p;
  status = LED_OFF;
  pattern = &OneLongBlink;
  position = 0;
  endtime = millis();
  Led::ledList.registerLed( this);
}

void Led::setStatus( int s) {
  status = s;
}

void Led::setPattern( int (*p)[20]){
  pattern = p;
  position = 0;
  endtime = millis();
}

void Led::update() {
  switch( status) {
    case LED_ON:
      digitalWrite( pin, HIGH);
      break;
    case LED_BLINK:
      if (millis() >= endtime) {
        position++;
        if( (*pattern)[position] == 0)
          position = 0;
        endtime += (*pattern)[position];
        if( position % 2 == 0)
          digitalWrite( pin, HIGH);
        else
          digitalWrite( pin, LOW);
      }
      break;
    case LED_OFF:
      digitalWrite( pin, LOW);
      break;
  }
}
