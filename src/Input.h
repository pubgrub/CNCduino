#include <Bounce2.h>

#ifndef INPUT_H
#define INPUT_H

#define INPUT_ON 1
#define INPUT_OFF 0

class Input {
  Bounce bounce;
  int status;
  bool changedSinceLastRead;
  bool changedSinceLastUpdate;
  long timeAtLastChange;

public:

  void attach( int input);
  int getStatus();
  int update();
  bool statusChanged();
  bool statusChangedOn();
  bool statusChangedOff();
  bool statusJustChanged();


};

#endif
