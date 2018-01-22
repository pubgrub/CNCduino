#include <InputList.h>

InputList::InputList() {
  inputs = 0;
}

void InputList::registerInput( Input *i) {
  input[ inputs] = i;
  inputs++;
}

void InputList::update() {
  for( int i = 0; i < inputs; i++) {
    input[ i]->update();
  }
}
