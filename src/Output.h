#ifndef OUTPUT_H
#define OUTPUT_H

#define OUTPUT_OFF 0
#define OUTPUT_ON 1
#define OUTPUT_BLINK 2
#define OUTPUT_BLINK_ONCE 3 // Output für bestimmte Zeit auf High halten

class OutputList;

class Output {
  int pin;
  int status; // OFF, ON, BLINK
  int (*pattern)[20];
  int position; // wo in pattern
  unsigned long endtime; // wann weiter im pattern

public:
  static OutputList outputList;


  Output( int p);
  void update();
  void setPattern( int (*pattern)[20]);
  void setStatus( int s);

};

#include <OutputList.h>

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
