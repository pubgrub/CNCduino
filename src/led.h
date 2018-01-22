#ifndef LED_H
#define LED_H

#define LED_OFF 0
#define LED_ON 1
#define LED_BLINK 2

class LedList;

class Led {
  int pin;
  int status; // OFF, ON, BLINK
  int (*pattern)[20];
  int position; // wo in pattern
  unsigned long endtime; // wann weiter im pattern

public:
  static LedList ledList;


  Led( int p);
  void update();
  void setPattern( int (*pattern)[20]);
  void setStatus( int s);

};

#include <LedList.h>

extern int OneShortBlinks[20];
extern int TwoShortBlinks[20];
extern int ThreeShortBlinks[20];

extern int OneLongBlink[20];

extern int Xyz_Blinks_0[20];
extern int Xyz_Blinks_1[20];
extern int Xyz_Blinks_2[20];
extern int Xyz_Blinks_3[20];
extern int Xyz_Blinks_12[20];
extern int Xyz_Blinks_13[20];
extern int Xyz_Blinks_23[20];
extern int Xyz_Blinks_123[20];

extern int *Blinks[8];


#endif
