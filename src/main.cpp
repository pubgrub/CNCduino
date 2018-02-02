#include <Arduino.h>
#include <Controllino.h>
#include <Bounce2.h>
#include <Output.h>
#include <Input.h>

// LEDs definieren

#define IO_ON_LED CONTROLLINO_D0
#define PH_ERR_LED CONTROLLINO_D4
#define FU_ERR_LED CONTROLLINO_D5
#define LIMIT_ERR_LED_RED CONTROLLINO_D1
#define LIMIT_ERR_LED_GREEN CONTROLLINO_D2
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

// sonstige Inputs definieren

// #define LIMIT_ERR_OVRD_STATUS CONTROLLINO XX

#define OFF 0
#define ON 1

const int X_ERR = 1;
const int Y_ERR = 2;
const int Z_ERR = 4;



// LED ************************

extern int *Blinks[8];


Output XyzErrLed = Output( XYZ_ERR_LED);
Output PhErrLed = Output( PH_ERR_LED);
Output FuErrLed = Output( FU_ERR_LED);
Output LimitErrLedRed = Output( LIMIT_ERR_LED_RED);
Output LimitErrLedGreen = Output( LIMIT_ERR_LED_GREEN);

// INPUTS ***********************

Input ioOnInput = Input( IO_ON_SW);
Input ioOffInput = Input( IO_OFF_SW);
Input xyzResetInput = Input( XYZ_RESET_SW);
Input limitOvrdInput = Input( LIMIT_OVRD_SW);

// INPUTS except Inputs

Input FuErrInput = Input( FU_ERR_SW);
Input PhErrInput = Input( PH_ERR_SW);
Input NotausErrInput = Input( NOTAUS_SW);
Input XErrInput = Input( X_ERR_SW);
Input YErrInput = Input( Y_ERR_SW);
Input ZErrInput = Input( Z_ERR_SW);
Input LimitErrInput = Input( LIMIT_ERR_SW);
// Input LimitErrOvrdStatus = Input( LIMIT_ERR_OVRD_STATUS);

// Fehler-Status OK=false, Fehler = true

bool FuErrStatus;
bool PHErrStatus;
bool NotausErrStatus;
bool XyzErrStatus;
int  XyzErrValue;
bool LimitErrStatus;

bool PriorityErrorStatus; // PH oder Notaus, haben Vorrang und disablen die anderen Errors

bool IoStatus;



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
    pinMode( LIMIT_ERR_LED_RED, OUTPUT);
    pinMode( LIMIT_ERR_LED_GREEN, OUTPUT);
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

  // Update Status der Inputs

  Input::inputList.update();

  // Update wirkliche Fehler-Status

  NotausErrStatus = NotausErrInput.getStatus();
  PHErrStatus = PhErrInput.getStatus();

  //Andere Fehler würden von diesen mit ausgelöst, werden deshalb unterdrückt
  PriorityErrorStatus = NotausErrStatus || PHErrStatus;

  FuErrStatus = ! PriorityErrorStatus && FuErrInput.getStatus();

  // Fehler der einzelnen Achsen auslesen
  int xyzErrors = 0;
  xyzErrors += XErrInput.getStatus() ? X_ERR : 0;
  xyzErrors += YErrInput.getStatus() ? Y_ERR : 0;
  xyzErrors += ZErrInput.getStatus() ? Z_ERR : 0;

  if( xyzErrors && ! PriorityErrorStatus) {
    XyzErrStatus = true;
    XyzErrValue = xyzErrors;
  }
  else {
    XyzErrStatus = false;
  }

  // Limit Fehler gerade neu aufgetreten
  if( LimitErrInput.statusChangedOn()){
    IoStatus = false;
  }




  LimitErrStatus = ! PriorityErrorStatus && LimitErrInput.getStatus();



  // Leds setzen
  XyzErrLed.setStatus( XyzErrStatus ? OUTPUT_BLINK : OUTPUT_OFF);
  PhErrLed.setStatus( PHErrStatus ? OUTPUT_ON : OUTPUT_OFF);
  FuErrLed.setStatus( FuErrStatus ? OUTPUT_ON : OUTPUT_OFF);


  // OUTPUTs updaten

  Output::outputList.update();

}
