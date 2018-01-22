#include <Button.h>
#include <ButtonList.h>
#include <Bounce2.h>

ButtonList Button::buttonList = ButtonList();

Button::Button( int input) {
  bounce = Bounce();
  bounce.attach( input);
  bounce.interval( 5);
  status = BUTTON_OFF;
  Button::buttonList.registerButton( this);
}

int Button::getStatus() {
  return status;
}

int Button::update() {
  changedSinceLastUpdate = false;
  bounce.update();
  int oldStatus = status;
  status = bounce.read();
  if( status != oldStatus) {
    changedSinceLastRead = true;
    changedSinceLastUpdate = true;
  }

  return status;
}

bool Button::statusChanged() {
  if( changedSinceLastRead) {
    changedSinceLastRead = false;
    return true;
  }
  return false;
}

bool Button::statusChangedOn() {
  if( changedSinceLastRead && ( status == BUTTON_ON )) {
    changedSinceLastRead = false;
    return true;
  }
  return false;
}

bool Button::statusChangedOff() {
  if( changedSinceLastRead && ( status == BUTTON_OFF )) {
    changedSinceLastRead = false;
    return true;
  }
  return false;
}
