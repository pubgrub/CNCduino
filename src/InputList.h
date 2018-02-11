#ifndef INPUTLIST_H
#define INPUTLIST_H

class Input;

class InputList {
  int inputs;
  Input* input[20];

public:
  InputList();
  void registerInput( Input* input);
  void update();
};

#include <Input.h>
#endif
