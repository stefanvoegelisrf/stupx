int pinLightBarrier = 2;
int counter = 0;
boolean lastLightBarrierStatus;
int loopCounter = 0;

void setup() {
  Serial.begin(9600);

  pinMode(pinLightBarrier, INPUT);
  lastLightBarrierStatus = digitalRead(pinLightBarrier);
}

void loop() {
  loopCounter++;
  boolean newLightBarrierStatus;
  newLightBarrierStatus = digitalRead(pinLightBarrier);

  // dedect CHANGE
  if (lastLightBarrierStatus != newLightBarrierStatus) {
    Serial.println("CHANGE");
  }

  // detect RISING
  if (int(newLightBarrierStatus - lastLightBarrierStatus) == 1) {
    // Serial.println("RISING");
    counter++;
  }

  // detect FALLING
  // if (int(newLightBarrierStatus - lastLightBarrierStatus) == -1) {
  //   Serial.println("FALLING");
  // }

  // remember LbStatus for next loop
  lastLightBarrierStatus = newLightBarrierStatus;
  if (loopCounter % 3000 == 0) {
    // feedback of counter
    Serial.print("counter ");
    Serial.println(counter, DEC);
  }
}
