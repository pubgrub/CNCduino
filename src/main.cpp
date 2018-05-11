#include <Arduino.h>
#include <Controllino.h>
#include <Bounce2.h>
#include <Output.h>
#include <Input.h>
#include <InputList.h>
#include <OutputList.h>

#define DEBUG 0
#define CALC_SPEED 1

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
#define MACH_OK_STATUS CONTROLLINO_A15

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

// Alle Variablen, die mit Limit Fehler zu tun haben

Output LimitErrLedRed;       // Rote LED in Limit-Err-Button
Output LimitErrLedGreen;     // Grüne LED in Limit-Err-Button
Output LimitErrRly;          // Output von Limit-Error-Status an Mach
Output LimitOvrdRly;         // Output von Limit-Error-Ovrd-Status an Mach
Input LimitOvrdInput;        // Status von Limit_Error-Ovrd-Button
Input LimitErrInput;         // Status von Limit-Error-Switch
Input LimitOvrdMachInput;    // Input von Limit-Ovrd-Status aus Mach
bool LimitErrStatus;         // Interner Status von Limit-Error (extra, weil Limit-Sw Öffner oder Schließer sein kann)
bool LimitErrOvrdStatus;     // Interner Status des Limit-Overrides
bool LimitOvrdMachStatus;    // Interner Status des Mach Ovrd Signals

// Alle Variablen, die mit XYZ-Fehlern zu tun haben

Output XyzErrLed;            // Gelbe LED im XYZ-Err-Button
Output XyzResetRly;          // Output von Xyz-Reset an Reset-Relay;
Input xyzResetInput;         // Status des Reset-Tasters
Input XErrInput;             // Status des X-Achsen-Fehlers
Input YErrInput;             // Status des Y-Achsen-Fehlers
Input ZErrInput;             // Status des Z-Achsen-Fehlers
int  XyzErrValue;            // Fehler-Status, bitweise kombiniert. 0=kein Fehler
bool XyzErrStatus;           // Echter Fehler-Status (nach Prio-Fehler-Check)
bool XyzResetStatus;         // Tatsächerlicher Reset-Status ( Button + Fehler)


// Alle Variablen, die mit PH-Fehlern zu tun haben

Output PhErrLed;             // Gelbe LED im PH-Err-Button
Input PhErrInput;            // Status des PH-Err-Switches
bool PHErrStatus;            // Interner Status des PH Errors


// Alle Variablen, die mit Notaus-Errors zu tun haben

Input NotausErrInput;        // Status des Notaus-Relais
bool NotausErrStatus;        // Interner Status des Notaus Errors


// Alle Variablen, die mit FU-Fehlern zu tun haben

Output FuErrLed;             // Gelbe LED im FU-Err-Button
Input FuErrInput;            // Fehler-Status des FU
bool FuErrStatus;            // Echter Status des FU-Fehler (nach Prio-Fehler-Check)


// Alle Variablen, die mit dem Enable / Reset System zu tun haben

bool InternEnableStatus;      // Interner "Enable"-Status
bool InternLimitOvrdEnableStatus; // System ist im LimOvrd Modus und "ON"-Switch wurde gedrückt
Output EnableLed;             // Weiße LED im "ON"-Switch
Input EnableOnInput;          // Status des weißen "ON"-Switches
Input EnableOffInput;         // Status des schwarzen "OFF"-Switches
Input MachOkInput;           // "Reset"-Status aus Mach
bool MachOkStatus;
Output EnableRly;             // "Enable"-Signal an Mach



// Andere Variablen

extern int *Blinks[8];
bool PriorityErrorStatus; // PH oder Notaus, haben Vorrang und disablen die anderen Errors



// Timing Startup

long TimestampStart;
bool isInitialized;
long NextDebugOutput;
long DebugInterval;
long Loops;
char buffer[80];


// void ioStatusUpdate( int status);
// void fuStatusUpdate();
// void phStatusUpdate();
// void notausStatusUpdate();
// void xyzStatusUpdate();


void setup() {

    // put your setup code here, to run once:
    Serial.begin(38400);

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
    pinMode( MACH_OK_STATUS, INPUT);

    pinMode( XYZ_RESET_RLY, OUTPUT);
    pinMode( ENABLE_RLY, OUTPUT);
    pinMode( LIMIT_ERR_RLY, OUTPUT);
    pinMode( LIMIT_OVRD_RLY, OUTPUT);

    // setup Input/Output instances

    // INPUTS ***********************

    EnableOnInput.attach( IO_ON_SW);
    EnableOffInput.attach( IO_OFF_SW);
    xyzResetInput.attach( XYZ_RESET_SW);
    LimitOvrdInput.attach( LIMIT_OVRD_SW);

    FuErrInput.attach( FU_ERR_SW);
    PhErrInput.attach( PH_ERR_SW);
    NotausErrInput.attach( NOTAUS_SW);
    XErrInput.attach( X_ERR_SW);
    YErrInput.attach( Y_ERR_SW);
    ZErrInput.attach( Z_ERR_SW);
    LimitErrInput.attach( LIMIT_ERR_SW);
    LimitOvrdMachInput.attach( LIMIT_OVRD_STATUS);
    MachOkInput.attach(MACH_OK_STATUS);

    // OUTPUTs

    XyzErrLed.attach( XYZ_ERR_LED);
    XyzResetRly.attach( XYZ_RESET_RLY);
    PhErrLed.attach( PH_ERR_LED);
    FuErrLed.attach( FU_ERR_LED);
    LimitErrLedRed.attach( LIMIT_ERR_LED_RED);
    LimitErrLedGreen.attach( LIMIT_ERR_LED_GREEN);
    LimitErrLedRed.setPattern( &OneLongBlink);
    LimitErrLedGreen.setPattern( &OneLongBlink);
    EnableRly.attach( ENABLE_RLY);
    EnableLed.attach( ENABLE_LED);
    EnableLed.setPattern( &OneLongBlink);
    LimitErrRly.attach(LIMIT_ERR_RLY);
    LimitOvrdRly.attach(LIMIT_OVRD_RLY);
    LimitOvrdRly.setPattern(&OneLongBlink);

    TimestampStart = millis();
    NextDebugOutput = TimestampStart;
    DebugInterval = 1000;
    Loops = 0;

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
  if( EnableOnInput.statusChangedOn()) {
    InternEnableStatus = true;
    if( LimitOvrdMachStatus) {
      InternLimitOvrdEnableStatus = true;
    }
  }

  if( ! EnableOffInput.getStatus()) { // Öffner, Normal HIGH
    InternEnableStatus = false;
    InternLimitOvrdEnableStatus = false;
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

  LimitErrStatus = LimitErrInput.getStatus(); // evtl invertieren, falls Öffner als Limitschalter genutzt werden.

  // Jeder der oberen Fehler schaltet IOStatus aus
  if( LimitErrStatus) {
    InternEnableStatus = false;
  }
  if( NotausErrStatus || PHErrStatus || XyzErrStatus) {
    InternEnableStatus = false;
    InternLimitOvrdEnableStatus = false;
  }



  // LimitErr an Mach melden
  LimitErrRly.setStatus(LimitErrStatus ? OUTPUT_ON : OUTPUT_OFF);

  // Limit Override Status setzen
  // und an mach senden (einmal "blinken")
  if( LimitErrStatus && LimitOvrdInput.statusChangedOn() && ! LimitErrOvrdStatus) {
    LimitErrOvrdStatus = true;
    LimitOvrdRly.setStatus( OUTPUT_BLINK_ONCE);
  }

  // LimitErr weg? Ovrd weg!
  if( LimitErrStatus == false) {
    LimitErrOvrdStatus = false;
    InternLimitOvrdEnableStatus = false;
  }

  // Hat Mach den Limit Ovrd mitbekommen?
  LimitOvrdMachStatus = ! LimitOvrdMachInput.getStatus();


  //Enable an Mach melden
  EnableRly.setStatus( InternEnableStatus || InternLimitOvrdEnableStatus ? OUTPUT_ON : OUTPUT_OFF);

  MachOkStatus = ! MachOkInput.getStatus();

  // Leds setzen
  if( XyzErrStatus) {
    XyzErrLed.setPattern( (int(*)[20]) Blinks[ XyzErrValue]);
  }
  XyzErrLed.setStatus( XyzErrStatus ? OUTPUT_BLINK : OUTPUT_OFF);

  PhErrLed.setStatus( PHErrStatus ? OUTPUT_ON : OUTPUT_OFF);
  FuErrLed.setStatus( FuErrStatus ? OUTPUT_ON : OUTPUT_OFF);



  if( MachOkStatus) {
    if( InternLimitOvrdEnableStatus) {
      EnableLed.setStatus( OUTPUT_BLINK);
    } else {
      EnableLed.setStatus( OUTPUT_ON);
    }
  } else {
    EnableLed.setStatus( OUTPUT_OFF);
  }

  if( LimitErrStatus) {
    if( LimitErrOvrdStatus) {
      if( InternLimitOvrdEnableStatus) {
        LimitErrLedRed.setStatus( OUTPUT_BLINK);
        LimitErrLedGreen.setStatus( OUTPUT_OFF);
      } else {
        LimitErrLedRed.setStatus( OUTPUT_ON);
        LimitErrLedGreen.setStatus( OUTPUT_OFF);
      }
    } else {
      LimitErrLedRed.setStatus( OUTPUT_ON);
      LimitErrLedGreen.setStatus( OUTPUT_ON);
    }
  } else {
    LimitErrLedRed.setStatus( OUTPUT_OFF);
    LimitErrLedGreen.setStatus( OUTPUT_OFF);
  }
  // OUTPUTs updaten

  outputList.update();

  if( DEBUG && (millis() > NextDebugOutput) ) {

    sprintf( buffer, "Zeitstempel: %ld", millis());
    Serial.println( buffer);
    sprintf( buffer, "LimErrSts   : %1d LimErrOvrSts: %1d LimOvrMchSts: %1d", LimitErrStatus, LimitErrOvrdStatus, LimitOvrdMachStatus);
    Serial.println( buffer);
    sprintf( buffer, "XYZErrSts   : %1d XYZErrValue : %1d XYZResetSts : %1d", XyzErrStatus, XyzErrValue, XyzResetStatus);
    Serial.println( buffer);
    sprintf( buffer, "PhaErrSts   : %1d NotausErrSts: %1d FUErrSts    : %1d", PHErrStatus, NotausErrStatus, FuErrStatus);
    Serial.println( buffer);
    sprintf( buffer, "IntEnableSts: %1d IntLimEnaSts: %1d MachOkSts   : %1d", InternEnableStatus, InternLimitOvrdEnableStatus, MachOkStatus);
    Serial.println( buffer);
    NextDebugOutput += DebugInterval;
  }

  Loops++;
  if( CALC_SPEED && (millis() > NextDebugOutput) ) {
    NextDebugOutput += DebugInterval;
    sprintf( buffer, "loops/s %d", Loops);
    Serial.println( buffer);
    Loops = 0;
  }

}
