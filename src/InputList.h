#ifndef INPUTLIST_H
#define INPUTLIST_H

class Input;

class InputList {
  int inputs;
  Input* input[10];

public:
  InputList();
  void registerInput( Input* input);
  void update();
};

#include <Input.h>
#endif
