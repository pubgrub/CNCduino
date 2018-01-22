#ifndef INPUT_H
#define INPUT_H

#define INPUT_OFF 0
#define INPUT_ON 1

class InputList;

class Input {
  int pin;
  int status; // OFF, ON
  bool changedSinceLastRead;
  bool changedSinceLastUpdate;


public:
  static InputList inputList;

  Input( int p);
  void update();
  int getStatus();
  bool statusChanged();
  bool statusChangedOn();
  bool statusChangedOff();
  bool statusJustChanged();

};

#include <InputList.h>

#endif
