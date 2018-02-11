#include <arduino.h>
#include <Output.h>
#include <OutputList.h>

extern OutputList outputList;

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


OutputList Output::outputList = OutputList();


void Output::attach( int p){
  pin =p;
  status = OUTPUT_OFF;
  pattern = &OneLongBlink;
  position = 0;
  endtime = millis();
  outputList.registerOutput( this);
}

void Output::setStatus( int s) {
  status = s;
}

void Output::setPattern( int (*p)[20]){
  pattern = p;
  position = 0;
  endtime = millis();
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
            digitalWrite( pin, LOW);
          }
        }
        endtime += (*pattern)[position];
        digitalWrite( pin, ( position % 2 == 0) ? HIGH : LOW);
      }
      break;
    case OUTPUT_OFF:
      digitalWrite( pin, LOW);
      break;
  }
}
