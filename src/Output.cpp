#include <arduino.h>
#include <Output.h>
#include <OutputList.h>


int OneShortBlinks[20] = { 800, 200};
int TwoShortBlinks[20] = { 600, 200, 200, 200};
int ThreeShortBlinks[20] = { 600, 200, 200, 200, 200, 200};

int OneLongBlink[20] = { 500, 500};


int Xyz_Blinks_0[20] = {};
int Xyz_Blinks_1[20] = { 200, 800};
int Xyz_Blinks_2[20] = { 200, 200, 200, 800};
int Xyz_Blinks_3[20] = { 200, 200, 200, 200, 200, 800};
int Xyz_Blinks_12[20] = { 200, 800,     200, 200, 200, 800};
int Xyz_Blinks_13[20] = { 200, 800,     200, 200, 200, 200, 200, 800};
int Xyz_Blinks_23[20] = { 200, 200, 200, 800,     200, 200, 200, 200, 200, 800};
int Xyz_Blinks_123[20] = { 200, 800,     200, 200, 200, 800,     200, 200, 200, 200, 200, 800};

int *Blinks[8] = {  Xyz_Blinks_0,  Xyz_Blinks_1,  Xyz_Blinks_2,  Xyz_Blinks_12,
                  Xyz_Blinks_3,  Xyz_Blinks_13, Xyz_Blinks_23, Xyz_Blinks_123 };;

extern OutputList outputList;

void Output::attach( int p){
  pin =p;
  status = OUTPUT_OFF;
  outputList.registerOutput( this);
}

int Output::getStatus() {
  return status;
}

void Output::setStatus( int s) {
  if( s != status) {
    status = s;
    if( status == OUTPUT_BLINK_ONCE || status == OUTPUT_BLINK) {
      endtime = millis() + (*pattern)[0];
      position = 0;
    }
  }
}

void Output::setPattern( int (*p)[20]) {
  if( p != pattern) {
    pattern = p;
    position = 0;
  }
}

void Output::update() {
  switch( status) {
    case OUTPUT_ON:
      digitalWrite( pin, HIGH);
      break;
    case OUTPUT_BLINK:
    case OUTPUT_BLINK_ONCE:
      if (millis() >= endtime) {
        position++;
        if( (*pattern)[position] == 0) {
          position = 0;
          if( status == OUTPUT_BLINK_ONCE) {
            status = OUTPUT_OFF;
          }
        }
        endtime += (*pattern)[position];
      }
      digitalWrite( pin, ( position % 2 == 0) ? HIGH : LOW);
      break;
    case OUTPUT_OFF:
      digitalWrite( pin, LOW);
      break;
  }
}
