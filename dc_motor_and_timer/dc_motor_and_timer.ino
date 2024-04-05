// no 3 Vcc to Vcc 5V
// no 4 Vo  to Digital Input Pin of MicroController
// no 5 GND to Ground Gnd

// this methode of counting offers BEST performance and HIGHEST SENSITIVITY
//
// make sure that you have no noise at input-Pin - you have to filter noise via hardware/electronics eg. a Schmitt Trigger
// make sure you have a proper powersupply for your Microcontroller ( best a battery )
// make sure you have no disturbing electronics near to your setup
// make sure your wires are proper connected

// Timer0 and Timer2 can only count from 0 to 255 - 8 Bit
// Timer1 can count to 65535 - 16 Bit
// Timer1 is also used to generate the PWM on the OC1A and OC1B - Pins - should be Pin 9 and 10 (ATM168)
// if you use this method of counting you can not us analogWrite at those Pins, digitalWrite still works fine
const int E1 = 3;  ///<Motor1 Speed

const int M1 = 4;  ///<Motor1 Direction

int pinLightBarrier = 5;  // LIghtBarrier, Photointerrupter -> you must use ATM168-Pin-T1 see p108 of ATM168 datasheet

int encoderHoles = 24;
bool isForward = true;
int speed = 100;

void setup() {
  // start serial communication
  Serial.begin(9600);
  // define pins

  pinMode(E1, OUTPUT);
  pinMode(M1, OUTPUT);
  // timer setup
  timer1SetExternalClock();
}

void loop() {
  int counterValue = readTCNT1();
  if (counterValue > encoderHoles) {
    writeTCNT1(0);
    // Set motor speed to 0
    analogWrite(E1, 0);
    // Change direction
    isForward = !isForward;
  }
  isForward ? M1_advance(speed) : M1_back(speed);
}

void M1_advance(char Speed)  ///<Motor1 Advance
{
  digitalWrite(M1, LOW);
  analogWrite(E1, Speed);
}

void M1_back(char Speed)  ///<Motor1 Back off
{
  digitalWrite(M1, HIGH);
  analogWrite(E1, Speed);
}

//
void timer1SetExternalClock(void) {
  // Timer 1 16 Bit, 0 - 65535
  //__________________________________TIMER1_for_Encoder_________________________________
  //the entire Timer-Register is rewritten, destroying all Arduino Timer-settings
  // that a done in the init() function - see wiring.c
  // http://code.google.com/p/arduino/source/browse/trunk/hardware/arduino/cores/arduino/wiring.c
  // no pwm / no analogWrite on Pin 9, 10
  // Timer1 Setting - Reset - this will also stop the timer
  TCCR1A = 0;
  TCCR1B = 0;
  TCCR1C = 0;
  writeTCNT1(0);  //
  TCCR1B |= (0 << WGM13) | (0 << WGM12);
  TCCR1A |= (0 << WGM11) | (0 << WGM10);
  //Timer1 Overflow Interrupt Disable
  TIMSK1 = 0;
  pinMode(pinLightBarrier, INPUT);  // pinLB must be 5
  // external clock on pin5 falling edge
  TCCR1B |= (1 << CS12) | (1 << CS11) | (0 << CS10);
}

// read and write 16Bit-Counter as shown in ATM168 - datasheet page 112

void writeTCNT1(unsigned int num) {
  unsigned char sreg;
  unsigned int i;
  if (num >= 0) {
    /* Save global interrupt flag */
    sreg = SREG;

    /* Disable interrupts */
    cli();
    // set new value
    TCNT1 = num;
    /* Restore global interrupt flag */
    SREG = sreg;
  }
}


unsigned int readTCNT1(void) {
  unsigned char sreg;
  unsigned int i;
  /* Save global interrupt flag */
  sreg = SREG;
  /* Disable interrupts */
  cli();
  /* Read TCNT1 into i */
  i = TCNT1;
  /* Restore global interrupt flag */
  SREG = sreg;
  return i;
}