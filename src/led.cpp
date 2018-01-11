#include <arduino.h>
#include <led.h>

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


void ledUpdate( Led &led) {
  switch( led.status) {
    case LED_ON:
      digitalWrite( led.pin, HIGH);
      break;
    case LED_BLINK:
      if (millis() >= led.endtime) {
        led.position++;
        if( (*led.pattern)[led.position] == 0)
          led.position = 0;
        led.endtime += (*led.pattern)[led.position];
        if( led.position % 2 == 0)
          digitalWrite( led.pin, HIGH);
        else
          digitalWrite( led.pin, LOW);
      }
      break;
    case LED_OFF:
      digitalWrite( led.pin, LOW);
      break;
  }
}


void ledBlinkStart( Led &led) {
  led.status = LED_BLINK;
  led.position = -1;
  led.endtime = millis();
  ledUpdate(led);
}
