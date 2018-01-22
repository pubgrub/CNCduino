#ifdef BUTTON_H
  #error CAN_H ist definiert
  #endif

#include <Button.h>

#ifndef BUTTONLIST_H
#define BUTTONLIST_H

class ButtonList {
  int buttons;
  Button* button[10];

public:
  ButtonList();
  void registerButton( Button* button);
  void update();
};

#endif
