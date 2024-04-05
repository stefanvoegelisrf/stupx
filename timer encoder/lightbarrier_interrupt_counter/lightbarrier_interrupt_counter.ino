// using a Sharp GP1A53HR
// the Infrared-Led is connected
// no 1 Anode via a 470 Ohm to 5V -> less then 10 mA
// no 2 Cathode to Gnd
// the Recievers Pins are connect
// no 3 Vcc to Vcc 5V
// no 4 Vo  to Digital Input Pin of MicroController
// no 5 GND to Ground Gnd
//
// Note: this Photointerrupter has a Schmitt Trigger as Output
// this reduces the sensitivity for noise and will always give back LOW or HIGH
//


// this methode of counting offers much better performance then countinously comparing the lightBarrier-Status in the loop script
//


int pinLB = 2; // LIghtBarrier, Photointerrupter -> you must use Pin 2 or 3 see http://arduino.cc/en/Reference/AttachInterrupt
int counter = 0;

void setup(){
        // start serial communication
        Serial.begin(9600);

        pinMode(pinLB,INPUT);

        attachInterrupt(0,countLbEvent,RISING); // count wenn Lightbarrier gets closed
        // index 0 will refer to pin 2
        // there the following modes
        //  attachInterrupt(0,countLBEvent,LOW);
        //  attachInterrupt(0,countLBEvent,CHANGE);
        //  attachInterrupt(0,countLBEvent,RISING);
        //  attachInterrupt(0,countLBEvent,FALLING);
}



void loop(){

        // feedback of counter
        Serial.print("counter ");
        Serial.println(counter,DEC);
}

// keep this function smart and short
// no parameters, no return value
void countLbEvent(void) {
        counter++;
}
