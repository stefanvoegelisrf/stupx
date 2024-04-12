/*
   This sketch controls a servo motor and a stepper motor blocking mode

   Connect STEP, DIR as indicated!
   Driverslots X and Y on the CNC shield are used

   Written by Gordan Savicic 2023

   This file may be redistributed under the terms of the MIT license.
   A copy of this license has been included with this distribution in the file LICENSE.

   Wiring:
    Power supply +5v -> Servo + pin (red cable)
    Power supply GND -> CNC Shield GND pin
    Servo Data pin (orange or yellow) -> SpinEn on CNC shield (D12)
    Servo GND -> GND on CNC Shield

    Stepper in Slot X (Pin 5 & 2)
*/


#include <Arduino.h>
#include "BasicStepperDriver.h"

// Motor steps per revolution. Most steppers are 200 steps or 1.8 degrees/step
#define MOTOR_STEPS 200
#define RPM 60

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


const float full_rotation_L = MOTOR_STEPS * MICROSTEPS * 14;

// Initialize the driver(s)
BasicStepperDriver stepperX(MOTOR_STEPS, DIR_X, STEP_X, SLEEP);
BasicStepperDriver stepperY(MOTOR_STEPS, DIR_Y, STEP_Y, SLEEP);

void setup() {

  Serial.begin(115200);
  // Pass some config to the instances and begin
  stepperX.begin(RPM, MICROSTEPS);
  stepperY.begin(RPM, MICROSTEPS);

  // if using enable/disable on ENABLE pin (active LOW) instead of SLEEP uncomment next line
  stepperX.setEnableActiveState(LOW);
  stepperY.setEnableActiveState(LOW);

  // attach the servo
  // energize coils
  stepperX.enable();
  stepperY.enable();
}

int stepperRotation = MOTOR_STEPS * MICROSTEPS * 4;  // motor steps to make the platform rotate one time
int stepDirection = 1;                               // Initial step direction (1 for increment, -1 for decrement)

void loop() {

  stepperX.enable();
  stepperY.enable();
  stepperX.startMove(stepperRotation);
  stepperY.startMove(stepperRotation);

  unsigned wait_time_micros = 1;
  while (wait_time_micros > 0) {
    int nextActionX = stepperX.nextAction();
    int nextActionY = stepperY.nextAction();
    wait_time_micros = nextActionX + nextActionY;
  }
  delay(500);
  stepperX.disable();
  stepperY.disable();
}