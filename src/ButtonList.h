#ifndef BUTTONLIST_H
#define BUTTONLIST_H

class Button;

class ButtonList {
  int buttons;
  Button* button[10];

public:
  ButtonList();
  void registerButton( Button* button);
  void update();
};

#include <Button.h>
#endif
