#include <arduino.h>
#include <Input.h>
#include <InputList.h>

InputList Input::inputList = InputList();

Input::Input( int p){
  pin =p;
  Input::inputList.registerInput( this);
}


void Input::update() {
  changedSinceLastUpdate = false;
  int oldStatus = status;
  status = digitalRead( pin);
  if( status != oldStatus) {
    changedSinceLastRead = true;
    changedSinceLastUpdate = true;
  }

}
