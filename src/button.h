#include <Bounce2.h>

#ifndef BUTTON_H
#define BUTTON_H

#define BUTTON_ON 1
#define BUTTON_OFF 0

class ButtonList;

class Button {
  Bounce bounce;
  int status;
  bool changedSinceLastRead;
  bool changedSinceLastUpdate;
  long timeAtLastChange;

public:
  static ButtonList buttonList;

  Button( int input);
  int getStatus();
  int update();
  bool statusChanged();
  bool statusChangedOn();
  bool statusChangedOff();
  bool statusJustChanged();


};

#include <ButtonList.h>


#endif
