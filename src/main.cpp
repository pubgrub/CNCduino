#include <Arduino.h>
#include <Controllino.h>
#include <Bounce2.h>
#include <Led.h>
#include <Button.h>
#include <Input.h>

// LEDs definieren

#define IO_ON_LED CONTROLLINO_D0
#define PH_ERR_LED CONTROLLINO_D4
#define FU_ERR_LED CONTROLLINO_D5
#define LIMIT_ERR_LED CONTROLLINO_D1
#define XYZ_ERR_LED CONTROLLINO_D3
#define SAUG_ON_LED CONTROLLINO_D6

// Schalter definieren

#define IO_ON_SW CONTROLLINO_A0
#define IO_OFF_SW CONTROLLINO_A1
#define LIMIT_OVRD_SW CONTROLLINO_A2
#define XYZ_RESET_SW CONTROLLINO_A3
#define SAUG_SW CONTROLLINO_A4
#define LIMIT_ERR_SW CONTROLLINO_A5
#define FU_ERR_SW CONTROLLINO_A7
#define PH_ERR_SW CONTROLLINO_A8
#define X_ERR_SW CONTROLLINO_A9
#define Y_ERR_SW CONTROLLINO_A10
#define Z_ERR_SW CONTROLLINO_A11
#define NOTAUS_SW CONTROLLINO_A13

// Relays definieren

#define XYZ_RESET_RLY CONTROLLINO_R1
#define ENABLE_RLY CONTROLLINO_R14
#define LIMIT_ERR_RLY CONTROLLINO_R15
#define LIMIT_OVRD_RLY CONTROLLINO_R13


#define OFF 0
#define ON 1

const int X_ERR = 1;
const int Y_ERR = 2;
const int Z_ERR = 4;



// LED ************************

extern int *Blinks[8];


Led xyzErrLed = Led( XYZ_ERR_LED);

// BUTTONS ***********************

Button ioOnButton = Button( IO_ON_SW);
Button ioOffButton = Button( IO_OFF_SW);
Button xyzResetButton = Button( XYZ_RESET_SW);

// INPUTS except Buttons

Input FuErrInput = Input( FU_ERR_SW);
Input PhErrInput = Input( PH_ERR_SW);
Input NotausErrInput = Input( NOTAUS_SW);
Input XErrInput = Input( X_ERR_SW);
Input YErrInput = Input( Y_ERR_SW);
Input ZErrInput = Input( Z_ERR_SW);

// Fehler-Status OK=false, Fehler = true

bool FuErrStatus;
bool PHErrStatus;


int Io_status;
int Fu_err_status;
int Ph_err_status;
int Notaus_err_status;
int Xyz_err_status;



void ioStatusUpdate( int status);
void fuStatusUpdate();
void phStatusUpdate();
void notausStatusUpdate();
void xyzStatusUpdate();




void ioStatusUpdate( int status){
  digitalWrite( IO_ON_LED, status);
  digitalWrite( ENABLE_RLY, status);
}

void fuStatusUpdate() {
  if( Ph_err_status == OFF && Notaus_err_status == OFF && Fu_err_status)
    digitalWrite( FU_ERR_LED, ON);
  else
    digitalWrite( FU_ERR_LED, OFF);
  if( Fu_err_status)
    Io_status = OFF;
}

void phStatusUpdate() {
  digitalWrite( PH_ERR_LED, Ph_err_status);
  if( Ph_err_status)
    Io_status = OFF;
}

void notausStatusUpdate() {
  if( Notaus_err_status)
    Io_status = OFF;
}

void xyzStatusUpdate() {
  if( Xyz_err_status) {

    int old_status = xyzErrLed.mode;
    if( old_status != Xyz_err_status) {
      xyzErrLed.pattern = (int(*)[20])Blinks[Xyz_err_status]  ;
      xyzErrLed.mode = Xyz_err_status;
      ledBlinkStart(xyzErrLed);
    }
    Io_status = OFF;
  }
  else {
    xyzErrLed.status = OFF;
    xyzErrLed.mode = 0;
    ledUpdate( xyzErrLed);
  }
}


void setup() {
    // put your setup code here, to run once:
    Serial.begin(9600);

    for( int i = 0; i < 8; i++) {
      int *b = Blinks[i];
      for( int j = 0; j < 20; j++) {
        Serial.print( b[j]    );
        Serial.print(" ");
//        Serial.print( *(Blinks[i] + j));
//        Serial.print(" ");
      }
      Serial.println();
    }

// Setup Pins as INPUT or OUTPUT

    pinMode( IO_ON_LED, OUTPUT);
    pinMode( PH_ERR_LED, OUTPUT);
    pinMode( FU_ERR_LED, OUTPUT);
    pinMode( LIMIT_ERR_LED, OUTPUT);
    pinMode( XYZ_ERR_LED, OUTPUT);
    pinMode( SAUG_ON_LED, OUTPUT);

    pinMode( IO_ON_SW, INPUT);
    pinMode( IO_OFF_SW, INPUT);
    pinMode( LIMIT_OVRD_SW, INPUT);
    pinMode( XYZ_RESET_SW, INPUT);
    pinMode( SAUG_SW, INPUT);
    pinMode( LIMIT_ERR_SW, INPUT);
    pinMode( FU_ERR_SW, INPUT);
    pinMode( PH_ERR_SW, INPUT);
    pinMode( X_ERR_SW, INPUT);
    pinMode( Y_ERR_SW, INPUT);
    pinMode( Z_ERR_SW, INPUT);
    pinMode( NOTAUS_SW, INPUT);

    pinMode( XYZ_RESET_RLY, OUTPUT);
    pinMode( ENABLE_RLY, OUTPUT);
    pinMode( LIMIT_ERR_RLY, OUTPUT);
    pinMode( LIMIT_OVRD_RLY, OUTPUT);

    Serial.print( "Start\n");

}


void loop() {

  // Update Status der Eingänge

Input::inputList.update();

// Update Status der Buttons

Button::buttonList.update();



  Fu_err_status = 1 - digitalRead( FU_ERR_SW);
  Ph_err_status = 1 - digitalRead( PH_ERR_SW);
  Notaus_err_status = 1 - digitalRead( NOTAUS_SW);

  Xyz_err_status = 0;
  if( ! digitalRead( X_ERR_SW))
    Xyz_err_status += X_ERR;
  if( ! digitalRead( Y_ERR_SW))
    Xyz_err_status += Y_ERR;
  if( ! digitalRead( Z_ERR_SW))
    Xyz_err_status += Z_ERR;


  fuStatusUpdate();
  phStatusUpdate();
  notausStatusUpdate();
  xyzStatusUpdate();

  ioStatusUpdate(Io_status);

  // LEDs updaten

  Led::ledList.update();
}
