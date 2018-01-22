#include <ButtonList.h>

ButtonList::ButtonList() {
  buttons = 0;
}

void ButtonList::registerButton( Button *b) {
  button[ buttons] = b;
  buttons++;
}

void ButtonList::update() {
  for( int i = 0; i < buttons; i++) {
    button[ i]->update();
  }
}
