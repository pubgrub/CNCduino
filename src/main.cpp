#include <Arduino.h>
#include <Controllino.h>
#include <Bounce2.h>

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

const int OFF = 0;
const int ON = 1;
const int BLINK = 2;

const int X_ERR = 1;
const int Y_ERR = 2;
const int Z_ERR = 4;


struct Led {
  int pin;
  int status; // OFF, ON, BLINK
  int mode; //Blinkmode
  int (*pattern)[20];
  int position; // wo in pattern
  unsigned long endtime; // wann weiter im pattern
};


int OneShortBlinks[20] = { 800, 200};
int TwoShortBlinks[20] = { 600, 200, 200, 200};
int ThreeShortBlinks[20] = { 600, 200, 200, 200, 200, 200};

int OneLongBlink[20] = { 500, 500};

int Xyz_Blinks_0[20] = {};
int Xyz_Blinks_1[20] = { 800, 200};
int Xyz_Blinks_2[20] = { 800, 200, 200, 200};
int Xyz_Blinks_3[20] = { 800, 200, 200, 200, 200, 200};
int Xyz_Blinks_12[20] = { 800, 200,     800, 200, 200, 200};
int Xyz_Blinks_13[20] = { 800, 200,     800, 200, 200, 200, 200, 200};
int Xyz_Blinks_23[20] = { 800, 200, 200, 200,     800, 200, 200, 200, 200, 200};
int Xyz_Blinks_123[20] = { 800, 200,     800, 200, 200, 200,     800, 200, 200, 200, 200, 200};

int *Blinks[8] = {  Xyz_Blinks_0,  Xyz_Blinks_1,  Xyz_Blinks_2,  Xyz_Blinks_12,
                    Xyz_Blinks_3,  Xyz_Blinks_13, Xyz_Blinks_23, Xyz_Blinks_123 };

Led Xyz_err_led;

Bounce Io_on_button = Bounce();
Bounce Io_off_button = Bounce();
Bounce Xyz_reset_button = Bounce();

int Io_status;
int Fu_err_status;
int Ph_err_status;
int Notaus_err_status;
int Xyz_err_status;

int Io_on_button_status;
int Io_off_button_status;
int Xyz_reset_button_status;

void ledUpdate( Led &led);
void ledBlinkStart( Led &led);
void ioStatusUpdate( int status);
void fuStatusUpdate();
void phStatusUpdate();
void notausStatusUpdate();
void xyzStatusUpdate();


void ledUpdate( Led &led) {
  switch( led.status) {
    case ON:
      digitalWrite( led.pin, HIGH);
      break;
    case BLINK:
      if (millis() >= led.endtime) {
        led.position++;
        if( (*led.pattern)[led.position] == 0)
          led.position = 0;
        led.endtime += (*led.pattern)[led.position];
        if( led.position % 2 == 0)
          digitalWrite( led.pin, HIGH);
        else
          digitalWrite( led.pin, LOW);
      }
      break;
    case OFF:
      digitalWrite( led.pin, LOW);
      break;
  }
}


void ledBlinkStart( Led &led) {
  led.status = BLINK;
  led.position = -1;
  led.endtime = millis();
  ledUpdate(led);
}

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

    int old_status = Xyz_err_led.mode;
    if( old_status != Xyz_err_status) {
      Xyz_err_led.pattern = (int)Blinks[Xyz_err_status]  ;
      Xyz_err_led.mode = Xyz_err_status;
      ledBlinkStart(Xyz_err_led);
    }
    Io_status = OFF;
  }
  else {
    Xyz_err_led.status = OFF;
    Xyz_err_led.mode = 0;
    ledUpdate( Xyz_err_led);
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

    Xyz_err_led.status = OFF;
    Xyz_err_led.pin = XYZ_ERR_LED;

    Io_on_button.attach(IO_ON_SW);
    Io_on_button.interval(5);

    Io_off_button.attach(IO_OFF_SW);
    Io_on_button.interval(5);

    Serial.print( "Start\n");

}


void loop() {

  // Update Status der Eingänge

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

  // Update Status der Buttons

  Io_on_button.update();
  Io_off_button.update();
  Xyz_reset_button.update();

  // lese Status der Buttons

  Io_on_button_status = Io_on_button.read();
  Io_off_button_status = Io_off_button.read();
  Xyz_reset_button_status = Xyz_reset_button.read();



  ledUpdate(Xyz_err_led);

  if( Io_on_button_status) {
    Io_status = 1;
  }

  if ( ! Io_off_button_status) {
    Io_status = 0;
  }

  if (  Xyz_reset_button_status) {

  }


  fuStatusUpdate();
  phStatusUpdate();
  notausStatusUpdate();
  xyzStatusUpdate();

  ioStatusUpdate(Io_status);

}
