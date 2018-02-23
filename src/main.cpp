#include <Arduino.h>
#include <Controllino.h>
#include <Bounce2.h>
#include <Output.h>
#include <Input.h>
#include <InputList.h>
#include <OutputList.h>

// LEDs definieren

#define ENABLE_LED CONTROLLINO_D0
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
#define LIMIT_OVRD_STATUS CONTROLLINO_A6
#define FU_ERR_SW CONTROLLINO_A7
#define PH_ERR_SW CONTROLLINO_A8
#define X_ERR_SW CONTROLLINO_A9
#define Y_ERR_SW CONTROLLINO_A10
#define Z_ERR_SW CONTROLLINO_A11
#define NOTAUS_SW CONTROLLINO_A13
#define RESET_STATUS CONTROLLINO_A14

// Relays definieren

#define XYZ_RESET_RLY CONTROLLINO_R0
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


// Lists

InputList inputList = InputList();
OutputList outputList = OutputList();


// LED ************************

extern int *Blinks[8];


Output XyzErrLed;
Output PhErrLed;
Output FuErrLed;
Output LimitErrLedRed;
Output LimitErrLedGreen;
Output EnableLed;

// Other Outputs

Output XyzResetRly;
Output EnableRly;
Output LimitErrRly;
Output LimitOvrdRly;

// INPUTS ***********************

Input ioOnInput;
Input ioOffInput;
Input xyzResetInput;
Input LimitOvrdInput;

Input FuErrInput;
Input PhErrInput;
Input NotausErrInput;
Input XErrInput;
Input YErrInput;
Input ZErrInput;
Input LimitErrInput;
Input LimitOvrdStatus;
Input ResetStatus;

// Fehler-Status OK=false, Fehler = true

bool FuErrStatus;
bool PHErrStatus;
bool NotausErrStatus;
bool XyzErrStatus;
int  XyzErrValue;
bool XyzResetStatus;
bool LimitErrStatus;


bool PriorityErrorStatus; // PH oder Notaus, haben Vorrang und disablen die anderen Errors
bool IoStatus;



int Fu_err_status;
int Ph_err_status;
int Notaus_err_status;
int Xyz_err_status;

// Timing Startup

int TimestampStart;
bool isInitialized;



// void ioStatusUpdate( int status);
// void fuStatusUpdate();
// void phStatusUpdate();
// void notausStatusUpdate();
// void xyzStatusUpdate();


void setup() {

    // put your setup code here, to run once:
    Serial.begin(9600);

    // for( int i = 0; i < 8; i++) {
    //   int *b = Blinks[i];
    //   for( int j = 0; j < 20; j++) {
    //     Serial.print( b[j]    );
    //     Serial.print(" ");
    //     Serial.print( *(Blinks[i] + j));
    //     Serial.print(" ");
    //   }
    //   Serial.println();
    // }

// Setup Pins as INPUT or OUTPUT

    pinMode( ENABLE_LED, OUTPUT);
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
    pinMode( LIMIT_OVRD_STATUS, INPUT);
    pinMode( FU_ERR_SW, INPUT);
    pinMode( PH_ERR_SW, INPUT);
    pinMode( X_ERR_SW, INPUT);
    pinMode( Y_ERR_SW, INPUT);
    pinMode( Z_ERR_SW, INPUT);
    pinMode( NOTAUS_SW, INPUT);
    pinMode( RESET_STATUS, INPUT);

    pinMode( XYZ_RESET_RLY, OUTPUT);
    pinMode( ENABLE_RLY, OUTPUT);
    pinMode( LIMIT_ERR_RLY, OUTPUT);
    pinMode( LIMIT_OVRD_RLY, OUTPUT);

    // setup Input/Output instances

    // INPUTS ***********************

    ioOnInput.attach( IO_ON_SW);
    ioOffInput.attach( IO_OFF_SW);
    xyzResetInput.attach( XYZ_RESET_SW);
    LimitOvrdInput.attach( LIMIT_OVRD_SW);

    FuErrInput.attach( FU_ERR_SW);
    PhErrInput.attach( PH_ERR_SW);
    NotausErrInput.attach( NOTAUS_SW);
    XErrInput.attach( X_ERR_SW);
    YErrInput.attach( Y_ERR_SW);
    ZErrInput.attach( Z_ERR_SW);
    LimitErrInput.attach( LIMIT_ERR_SW);
    LimitOvrdStatus.attach( LIMIT_OVRD_STATUS);
    ResetStatus.attach(RESET_STATUS);

    // OUTPUTs

    XyzErrLed.attach( XYZ_ERR_LED);
    XyzResetRly.attach( XYZ_RESET_RLY);
    PhErrLed.attach( PH_ERR_LED);
    FuErrLed.attach( FU_ERR_LED);
    LimitErrLedRed.attach( LIMIT_ERR_LED_RED);
    LimitErrLedGreen.attach( LIMIT_ERR_LED_GREEN);
    EnableRly.attach( ENABLE_RLY);
    EnableLed.attach( ENABLE_LED);
    LimitErrRly.attach(LIMIT_ERR_RLY);
    LimitOvrdRly.attach(LIMIT_OVRD_RLY);

    // TimestampStart = millis();
    // int oneSecBlink[20] = { 1000, 0};
    // XyzResetRly.setPattern( &oneSecBlink);
    // XyzResetRly.setStatus(OUTPUT_BLINK_ONCE);
    //
    // while( XyzResetRly.getStatus()) {
    //   Serial.println( XyzResetRly.getStatus());
    //   outputList.update();
    // }
    Serial.print( "Start\n");

}


void loop() {

    // Update Status der Eingänge

  inputList.update();

  // Einschalter gerade gedrückt?
  // Wenn ja, erstmal einschalten, und falls Fehler aktiv, wird vor update wieder ausgeschalten
  if( ioOnInput.statusChangedOn()) {
    IoStatus = true;
  }

  if( ! ioOffInput.getStatus()) { // Öffner, Normal HIGH
    IoStatus = false;
  }

  // Update wirkliche Fehler-Status

  NotausErrStatus = ! NotausErrInput.getStatus(); // Normal HIGH
  PHErrStatus = ! PhErrInput.getStatus(); // Normal HIGH

  //Andere Fehler würden von diesen mit ausgelöst, werden deshalb unterdrückt
  PriorityErrorStatus = NotausErrStatus || PHErrStatus;

  FuErrStatus = ! ( PriorityErrorStatus || FuErrInput.getStatus() ); // Beide Normal HIGH

  // Fehler der einzelnen Achsen auslesen
  XyzErrValue = 0;
  XyzErrValue += XErrInput.getStatus() ? 0 : X_ERR; // Alle Normal HIGH
  XyzErrValue += YErrInput.getStatus() ? 0 : Y_ERR; //
  XyzErrValue += ZErrInput.getStatus() ? 0 : Z_ERR; //

  XyzErrStatus = ( XyzErrValue && ! PriorityErrorStatus) ? true : false;

  XyzResetStatus = XyzErrStatus && xyzResetInput.getStatus();
  XyzResetRly.setStatus( XyzResetStatus ? OUTPUT_ON : OUTPUT_OFF);

  // Jeder der oberen Fehler schaltet IOStatus aus
  if( NotausErrStatus || PHErrStatus || XyzErrStatus) {
    IoStatus = false;
  }

  // Limit Fehler gerade neu aufgetreten
  if( LimitErrInput.statusChangedOn()){
    IoStatus = false;
  }

  LimitErrStatus = LimitErrInput.getStatus(); // evtl invertieren, falls Öffner als Limitschalter genutzt werden.

  LimitErrRly.setStatus(LimitErrStatus ? OUTPUT_ON : OUTPUT_OFF);

  LimitOvrdRly.setStatus(LimitOvrdInput.getStatus() ? OUTPUT_ON : OUTPUT_OFF);


  //Enable setzen
  EnableRly.setStatus( IoStatus ? OUTPUT_ON : OUTPUT_OFF);


  // Leds setzen
  if( XyzErrStatus) {
    XyzErrLed.setPattern( (int(*)[20]) Blinks[ XyzErrValue]);
  }
  XyzErrLed.setStatus( XyzErrStatus ? OUTPUT_BLINK : OUTPUT_OFF);
  PhErrLed.setStatus( PHErrStatus ? OUTPUT_ON : OUTPUT_OFF);
  FuErrLed.setStatus( FuErrStatus ? OUTPUT_ON : OUTPUT_OFF);
  EnableLed.setStatus( IoStatus ? OUTPUT_ON : OUTPUT_OFF);

  // OUTPUTs updaten

  outputList.update();

}
