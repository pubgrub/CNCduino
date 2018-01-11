#include <arduino.h>
#include <led.h>


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
