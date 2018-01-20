#ifndef Led_h
#define Led_h

#define LED_OFF 0
#define LED_ON 1
#define LED_BLINK 2


class Led {
  int pin;
  int status; // OFF, ON, BLINK
  int oldStatus;
  int mode; //Blinkmode
  int oldMode;
  int (*pattern)[20];
  int position; // wo in pattern
  unsigned long endtime; // wann weiter im pattern

public:
  Led( int p);
  void update();
  void blinkStart();

};


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
