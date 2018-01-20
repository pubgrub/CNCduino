#include <Button.h>
#include <Bounce2.h>

Button::Button( int input) {
  bounce = Bounce();
  bounce.attach( input);
  bounce.interval( 5);
}

int Button::getStatus() {
  return status;
}

void Button::update(){
  bounce.update();
  oldStatus = status;
  status = bounce.read();
}
