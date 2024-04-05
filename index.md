# Studio Prototyping Extended

## Briefing

## Project
- React to microphone input, vary settings based on this
- Create the sketch with p5 and provide instructions via serial to Arduino
  - See: [github.com - Servobot](https://github.com/fleshgordo/stuprox/tree/main/08_Servobot)

## 27.03.2024
- [github.com - STUPROX](https://github.com/fleshgordo/stuprox)

### Motor types
Below is a list of the relevant motor types for this module.

#### Stepper Motor
- **Targeted Position**: Stepper motors move in discrete steps, allowing precise control of the angular position. They are often used in applications requiring accurate positioning and repeatability. A recent advancement includes a low-consumption stepper motor controller that can respond in real-time to target position changes, which is particularly useful in precision measurement and manufacturing.

> Explanation: [youtube.com - How does a stepper motor work](https://www.youtube.com/watch?v=eyqwLiowZiU&ab_channel=Lesics)

#### Servo Motor
- **Range from 0 to 180 Degrees**: Servo motors are capable of precise position control within a range of 0 to 180 degrees. They operate on pulse width modulation (PWM), where the width of the pulse dictates the angle to which the servo will turn. The servo's circuitry is designed to maintain its position stable until a new signal is received.

> Explanation: [youtube.com - How does a servo motor work](https://www.youtube.com/watch?v=tHOH-bYjR4k&ab_channel=TheEngineeringMindset)

#### DC Motor
- **Operation According to Voltage**: DC motors turn at a speed proportional to the voltage applied to them. The fundamental operation of a DC motor is based on the principle that when a current-carrying conductor is placed in a magnetic field, it experiences a mechanical force. By varying the voltage, or by changing the magnetic field strength, the speed of the motor can be controlled.

> Explanation: [youtube.com - How does a DC motor work](https://www.youtube.com/watch?v=1AaUK6pT_cE&ab_channel=TheEngineeringMindset)

### How to structure an ARDUINO sketch
- Hardware settings like `define`, `define` are macro definitions that allow constant values
TODO: define structure

### What programming language does Arduino use?
Arduino is programmed with a c/c++ 'dialect'. Most c/c++ will work but much of the standard libraries will not work. 

### Basic stepper introduction
> See: [github.com - 00_StepperIntro.ino](https://github.com/fleshgordo/stuprox/blob/main/00_StepperIntro/00_StepperIntro.ino)

This introduction is based on the [github.com - BasicStepperDriver.h](https://github.com/laurb9/StepperDriver/blob/master/src/BasicStepperDriver.h).

We have to define what stepper motor we access on the driver shield:
```c++
// Driver in CNC shield X
#define DIR_X 5
#define STEP_X 2
```

We also have to define the motor steps and rpm
```c++
// Motor steps per revolution. Most steppers are 200 steps or 1.8 degrees/step
#define MOTOR_STEPS 200
#define RPM 120
```

We also have to set the microsteps that are configured on the board:
```c++
// Since microstepping is set externally, make sure this matches the selected mode
// Set the jumper to middle position when using MICROSTEPS 4, no jumper = MICROSTEPS 1
// 1=full step, 2=half step etc.
#define MICROSTEPS 4
```

Now we can also define a sleep value for the motor and the initialize it:
```c++
// Define the pin for enable/disable functionality
#define SLEEP 8

// Initialize the driver(s)
BasicStepperDriver stepperX(MOTOR_STEPS, DIR_X, STEP_X, SLEEP);
BasicStepperDriver stepperY(MOTOR_STEPS, DIR_Y, STEP_Y, SLEEP);
```

In our setup function, we configure the stepper and configure which logic state on ENABLE pin means active:
```c++
void setup() {
  // Pass some config to the instances and begin
  stepperX.begin(RPM, MICROSTEPS);

  stepperY.begin(RPM, MICROSTEPS);

  // set speed profile with acceleration
  //stepper.setSpeedProfile(stepper.LINEAR_SPEED, 500, 500);

  // if using enable/disable on ENABLE pin (active LOW) instead of SLEEP uncomment next line
  stepperX.setEnableActiveState(LOW);
  stepperY.setEnableActiveState(LOW);
}
```

In our loop, we then enable the stepper, and in our while loop, rotate them after each other, step by step. Like that we get almost simultaneous. It is not really simultaneous as the rotate is a blocking action.
```c++
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

  delay(2000);  // repeat after 2sec. pause
}
```

### Using servo
This sample code can be used to use a servo:
```c++
#include <Servo.h>

/*  This sketch controls a servo from 0 to 180degrees and back
    Adapt to your needs. 

    Wiring:
    Power supply +5v -> Servo + pin (red cable)
    Power supply GND -> CNC Shield GND pin
    Servo Data pin (orange or yellow) -> SpinEn on CNC shield
    Servo GND -> GND on CNC Shield
*/

Servo servo;

const byte servo_pin = 12;  // connect to Spindle enable pin (SpinEn) on CNC shield.
float pos = 0;

const int servo_min_ms = 800;
const int servo_max_ms = 2100;

const int servo_min_pos = 55; // 55
const int servo_max_pos = 135; // 135
const int servo_center = 90;

float angle = 0.0;
float speed = 0.01;

void setup() {
  Serial.begin(115200);
  servo.attach(servo_pin);
  // uncomment to fine-tune PWM signal
  //servo.attach(servo_pin, servo_min_ms, servo_max_ms);
  servo.write(servo_center);
}


void loop() {
  int pos = 90 + sin(angle) * 90;
  //Serial.println(pos);
  servo.write(pos);
  delay(5);
  angle += speed;
}
```


### Rhino intro

#### Commands
We are interacting with a command interface and can interact with it by typing commands

#### Relative coordinates

#### CurveBoolean to combine curves

#### Trim

#### Join

#### Selection

#### https://wiki.mcneel.com/rhino/pictureframes

#### Diameter

#### Fingerzinkenverbindung

https://www.bastelgarage.ch/optischer-endschalter-lichtschranke-mit-kabel

### Creating gears

https://hessmer.org/gears/InvoluteSpurGearBuilder.html?circularPitch=4.71238898038469&pressureAngle=20&clearance=0.05&backlash=0.05&profileShift=0&gear1ToothCount=64&gear1CenterHoleDiamater=3&gear2ToothCount=32&gear2CenterHoleDiamater=3&showOption=3&gear1PitchCircleDiameter=20

### Motiongen