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



float angle = 0.0;
float speed = 0.01;
float pos = 0.0;

int final_pos = MOTOR_STEPS * MICROSTEPS * 5;
bool toggle_pen = false;

long randNumber;

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
  stepperX.setSpeedProfile(BasicStepperDriver::LINEAR_SPEED, 600, 600);
  stepperY.setSpeedProfile(BasicStepperDriver::LINEAR_SPEED, 600, 600);
  stepperX.enable();
  stepperY.enable();
  randomSeed(analogRead(0));
}

void loop() {
  int randomXRotation = random(5, 90);
  stepperX.rotate(randomXRotation);
  int randomYRotation = random(5, 90);
  stepperY.rotate(randomYRotation);
  delay(1);
}

static int SEED = 0;

static int hash[] = { 208, 34, 231, 213, 32, 248, 233, 56, 161, 78, 24, 140, 71, 48, 140, 254, 245, 255, 247, 247, 40,
                      185, 248, 251, 245, 28, 124, 204, 204, 76, 36, 1, 107, 28, 234, 163, 202, 224, 245, 128, 167, 204,
                      9, 92, 217, 54, 239, 174, 173, 102, 193, 189, 190, 121, 100, 108, 167, 44, 43, 77, 180, 204, 8, 81,
                      70, 223, 11, 38, 24, 254, 210, 210, 177, 32, 81, 195, 243, 125, 8, 169, 112, 32, 97, 53, 195, 13,
                      203, 9, 47, 104, 125, 117, 114, 124, 165, 203, 181, 235, 193, 206, 70, 180, 174, 0, 167, 181, 41,
                      164, 30, 116, 127, 198, 245, 146, 87, 224, 149, 206, 57, 4, 192, 210, 65, 210, 129, 240, 178, 105,
                      228, 108, 245, 148, 140, 40, 35, 195, 38, 58, 65, 207, 215, 253, 65, 85, 208, 76, 62, 3, 237, 55, 89,
                      232, 50, 217, 64, 244, 157, 199, 121, 252, 90, 17, 212, 203, 149, 152, 140, 187, 234, 177, 73, 174,
                      193, 100, 192, 143, 97, 53, 145, 135, 19, 103, 13, 90, 135, 151, 199, 91, 239, 247, 33, 39, 145,
                      101, 120, 99, 3, 186, 86, 99, 41, 237, 203, 111, 79, 220, 135, 158, 42, 30, 154, 120, 67, 87, 167,
                      135, 176, 183, 191, 253, 115, 184, 21, 233, 58, 129, 233, 142, 39, 128, 211, 118, 137, 139, 255,
                      114, 20, 218, 113, 154, 27, 127, 246, 250, 1, 8, 198, 250, 209, 92, 222, 173, 21, 88, 102, 219 };

int noise2(int x, int y) {
  int tmp = hash[(y + SEED) % 256];
  return hash[(tmp + x) % 256];
}

float lin_inter(float x, float y, float s) {
  return x + s * (y - x);
}

float smooth_inter(float x, float y, float s) {
  return lin_inter(x, y, s * s * (3 - 2 * s));
}

float noise2d(float x, float y) {
  int x_int = x;
  int y_int = y;
  float x_frac = x - x_int;
  float y_frac = y - y_int;
  int s = noise2(x_int, y_int);
  int t = noise2(x_int + 1, y_int);
  int u = noise2(x_int, y_int + 1);
  int v = noise2(x_int + 1, y_int + 1);
  float low = smooth_inter(s, t, x_frac);
  float high = smooth_inter(u, v, x_frac);
  return smooth_inter(low, high, y_frac);
}

float perlin2d(float x, float y, float freq, int depth) {
  float xa = x * freq;
  float ya = y * freq;
  float amp = 1.0;
  float fin = 0;
  float div = 0.0;

  int i;
  for (i = 0; i < depth; i++) {
    div += 256 * amp;
    fin += noise2d(xa, ya) * amp;
    amp /= 2;
    xa *= 2;
    ya *= 2;
  }

  return fin / div;
}