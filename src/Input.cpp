#include <arduino.h>
#include <Input.h>
#include <InputList.h>

InputList Input::inputList = InputList();

Input::Input( int p, bool inv = false) {
  pin =p;
  invert = inv;
  Input::inputList.registerInput( this);
}


void Input::update() {
  changedSinceLastUpdate = false;
  bool oldStatus = status;
  status = (digitalRead( pin) != invert) ? true : false; // pin xor invertiert
  if( status != oldStatus) {
    changedSinceLastRead = true;
    changedSinceLastUpdate = true;
  }

}

bool Input::getStatus() {
  return status;
}
