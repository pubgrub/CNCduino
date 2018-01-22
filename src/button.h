#include <Bounce2.h>
#include <ButtonList.h>

#ifndef BUTTON_H
#define BUTTON_H

#define BUTTON_ON 1
#define BUTTON_OFF 0

class Button {
  Bounce bounce;
  int status;
  bool changedSinceLastRead;
  bool changedSinceLastUpdate;
  static ButtonList buttonList;

public:
  Button( int input);
  int getStatus();
  int update();
  bool statusChanged();
  bool statusChangedOn();
  bool statusChangedOff();
  bool statusJustChanged();


};



#endif
