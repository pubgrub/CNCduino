#include <Button.h>
#include <Bounce2.h>

Button::Button( int input) {
  bounce = Bounce();
  bounce.attach( input);
  bounce.interval( 5);
  oldStatus = BUTTON_OFF;
  status = BUTTON_OFF;
}

int Button::getStatus() {
  return status;
}

int Button::update() {
  bounce.update();
  oldStatus = status;
  status = bounce.read();
  return status;
}

bool Button::statusChanged() {
  return ( oldStatus != status) ? true : false;
}

bool Button::statusChangedOn() {
  return (status == BUTTON_ON && oldStatus == BUTTON_OFF) ? true : false;
}

bool Button::statusChangedOff() {
  return (status == BUTTON_OFF && oldStatus == BUTTON_ON) ? true : false;
}
