#include <Bounce2.h>

#ifndef BUTTON_H
#define BUTTON_H

#define BUTTON_ON 1
#define BUTTON_OFF 0

// struct Button {
//   Bounce bounce;
//   int status; // OFF, ON
//   int oldStatus; // OFF, ON
// };

class Button {
  Bounce bounce;
  int status;  //OFF, ON
  int oldStatus; //OFF, ON


public:
  Button( int input);
  int getStatus();
  int update();
  bool statusChanged();
  bool statusChangedOn();
  bool statusChangedOff();


};



#endif
