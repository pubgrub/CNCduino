#include <Bounce2.h>

#ifndef BUTTON_H
#define BUTTON_H



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
  void update();
  

};



#endif
