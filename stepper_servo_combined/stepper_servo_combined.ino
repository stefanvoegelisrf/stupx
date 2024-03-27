#include <Servo.h>
#include <Arduino.h>
#include "BasicStepperDriver.h"

// Motor steps per revolution. Most steppers are 200 steps or 1.8 degrees/step
#define MOTOR_STEPS 200
#define RPM 120

// Since microstepping is set externally, make sure this matches the selected mode
// Set the jumper to middle position when using MICROSTEPS 4, no jumper = MICROSTEPS 1
// 1=full step, 2=half step etc.
#define MICROSTEPS 4

// Driver in CNC shield X
#define DIR_X 5
#define STEP_X 2

// Driver in CNC shield Y
#define DIR_Y 6
#define STEP_Y 3

// Driver in CNC shield Z
#define DIR_Z 7
#define STEP_Z 4

// Define the pin for enable/disable functionality
#define SLEEP 8

// Initialize the driver(s)
BasicStepperDriver stepperX(MOTOR_STEPS, DIR_X, STEP_X, SLEEP);
BasicStepperDriver stepperY(MOTOR_STEPS, DIR_Y, STEP_Y, SLEEP);

Servo servo;

const byte servo_pin = 12;  // connect to Spindle enable pin (SpinEn) on CNC shield.
float pos = 0;

const int servo_min_ms = 800;
const int servo_max_ms = 2100;

const int servo_min_pos = 55;   // 55
const int servo_max_pos = 135;  // 135
const int servo_center = 90;

float angle = 0.0;
float speed = 0.01;

void setup() {
  Serial.begin(115200);
  // put your setup code here, to run once:
  servo.attach(servo_pin);
  // uncomment to fine-tune PWM signal
  //servo.attach(servo_pin, servo_min_ms, servo_max_ms);
  servo.write(servo_center);

  // Pass some config to the instances and begin
  stepperX.begin(RPM, MICROSTEPS);

  stepperY.begin(RPM, MICROSTEPS);

  // set speed profile with acceleration
  //stepper.setSpeedProfile(stepper.LINEAR_SPEED, 500, 500);

  // if using enable/disable on ENABLE pin (active LOW) instead of SLEEP uncomment next line
  stepperX.setEnableActiveState(LOW);
  stepperY.setEnableActiveState(LOW);
}

void loop() {
  // energize coils
  stepperX.enable();
  stepperY.enable();

  // Moving motor one full revolution using the degree notation

  int totalRotation = 0;
  while (totalRotation < 360) {
    stepperX.rotate(1);
    stepperY.rotate(1);
    totalRotation++;
  }


  // pause and allow the motor to be moved by hand
  stepperX.disable();
  stepperY.disable();

  int pos = 90 + sin(angle) * 90;
  Serial.println(pos);
  // servo.write(pos);
  angle += speed;

}
