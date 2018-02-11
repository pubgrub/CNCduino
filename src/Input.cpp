#include <Arduino.h>
#include <Input.h>
#include <InputList.h>
#include <Bounce2.h>

extern InputList inputList;

void Input::attach( int input) {
  bounce = Bounce();
  bounce.attach( input);
  bounce.interval( 5);
  status = INPUT_OFF;
  inputList.registerInput( this);
  timeAtLastChange = millis();
}

int Input::getStatus() {
  return status;
}

int Input::update() {
  changedSinceLastUpdate = false;
  bounce.update();
  int oldStatus = status;
  status = bounce.read();
  if( status != oldStatus) {
    changedSinceLastRead = true;
    changedSinceLastUpdate = true;
    timeAtLastChange = millis();
  }

  return status;
}

bool Input::statusChanged() {
  if( changedSinceLastRead) {
    changedSinceLastRead = false;
    return true;
  }
  return false;
}

bool Input::statusChangedOn() {
  if( changedSinceLastRead && ( status == INPUT_ON )) {
    changedSinceLastRead = false;
    return true;
  }
  return false;
}

bool Input::statusChangedOff() {
  if( changedSinceLastRead && ( status == INPUT_OFF )) {
    changedSinceLastRead = false;
    return true;
  }
  return false;
}
