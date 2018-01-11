#include <Bounce2.h>

struct Button {
  Bounce bounce;
  int status; // OFF, ON
  int oldStatus; // OFF, ON
};
