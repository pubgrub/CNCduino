struct Led {
  int pin;
  int status; // OFF, ON, BLINK
  int oldStatus;
  int mode; //Blinkmode
  int oldMode;
  int (*pattern)[20];
  int position; // wo in pattern
  unsigned long endtime; // wann weiter im pattern
};

int *Blinks[8];
