#include <IRremote.h>
#include <avr/sleep.h>
#define interruptPin 18

const int stepPinTop = 2;
const int dirPinTop = 3;
const int stepPinDown = 4;
const int dirPinDown = 5;
const int pumpEnable = 8;
const int irReceivePin = 11;
const int forwardButton = 22;
const int stopButton = 24;
const int backwardButton = 26;
const int limitButtonRight = 34;
const int limitButtonLeft = 36;
int customDelayMapped; // Defines variables

boolean stopStep = true;
char dirValue = ' ';

void setup() {
  Serial.begin(115200);
  // Sets the two pins as Outputs
  pinMode(stepPinTop, OUTPUT);
  pinMode(stepPinDown, OUTPUT);
  pinMode(dirPinTop, OUTPUT);
  pinMode(dirPinDown, OUTPUT);
  pinMode(pumpEnable, OUTPUT);
  pinMode(forwardButton, INPUT);
  pinMode(stopButton, INPUT);
  pinMode(forwardButton, INPUT);
  pinMode(limitButtonRight, INPUT);
  pinMode(limitButtonLeft, INPUT);

  // IR initialisation
  IrReceiver.begin(irReceivePin, ENABLE_LED_FEEDBACK, USE_DEFAULT_FEEDBACK_LED_PIN);
  // initial dir
  stepMove('f');
  // disiable pump
  digitalWrite(pumpEnable, HIGH);

  /**
   * Power interrupt init
   */
   pinMode(LED_BUILTIN, OUTPUT);
   pinMode(interruptPin, INPUT_PULLUP);
   digitalWrite(LED_BUILTIN, HIGH);
}
void loop() {

  if (!stopStep) {
    runStepper();
  }

  irListener();
  buttonsListener();
  limitSensorsListener();

  if (Serial.available() > 0) {
    char inByte = Serial.read();

    Serial.print("Value: ");
    Serial.println(inByte);

    if (inByte == 's') {
      delay(1000);
      Serial.println("-- Go To Sleep --");
      digitalWrite(pumpEnable, LOW);
      delay(1000);
      digitalWrite(pumpEnable, HIGH);
//      goingToSleep();      
    }
  }
  
//  Serial.print("Speed: ");
//  Serial.println(customDelayMapped);
}
// Function for reading the Potentiometer
int speedUp() {
  int customDelay = analogRead(A0); // Reads the potentiometer
  int newCustom = map(customDelay, 0, 1023, 200, 800);
  return newCustom;  
}

void runStepper() {
  customDelayMapped = speedUp();
  
  digitalWrite(stepPinTop, HIGH);
  digitalWrite(stepPinDown, HIGH);
  delayMicroseconds(customDelayMapped);
  digitalWrite(stepPinTop, LOW);
  digitalWrite(stepPinDown, LOW);
  delayMicroseconds(customDelayMapped);
}

void stepMove(char dir) {
  // f = forward; b = backward
  if (dir == 'f' || dir == 'F') {
    // Forward direction
    digitalWrite(dirPinTop, HIGH);
    digitalWrite(dirPinDown, LOW);
  } else {
    digitalWrite(dirPinTop, LOW);
    digitalWrite(dirPinDown, HIGH);
  }
}

void stepControlWithoutPump(boolean stopControl, char dirControl) {
  stopStep = stopControl;
  stepMove(dirControl);
  dirValue = dirControl;
  digitalWrite(pumpEnable, HIGH);
  delay(50);
}

void stepControlWithPump(boolean stopControl, char dirControl) {
  stopStep = stopControl;
  stepMove(dirControl);
  dirValue = dirControl;
  if (stopControl) {
    digitalWrite(pumpEnable, HIGH);
  } else {
    digitalWrite(pumpEnable, LOW);
    delay(100);
  }
  delay(50);
}

void irListener() {
  if (IrReceiver.decode()) {
    IrReceiver.printIRResultShort(&Serial);
    if (IrReceiver.decodedIRData.protocol == UNKNOWN) {
      // We have an unknown protocol here, print more info
      IrReceiver.printIRResultRawFormatted(&Serial, true);
    }
    
    Serial.println();
    // Enable receiving of the next value
    IrReceiver.resume();
        
    /*
     * Finally, check the received data and perform actions according to the received command
     */
    if (IrReceiver.decodedIRData.command == 0x8) {
        // forward without pump
        stepControlWithoutPump(false, 'f');
        
    } else if (IrReceiver.decodedIRData.command == 0x5A) {
        // backward without pump
        stepControlWithoutPump(false, 'b');
        
    } else if (IrReceiver.decodedIRData.command == 0x1C) {
        // stop without pump
        stepControlWithoutPump(true, dirValue);

    } else if (IrReceiver.decodedIRData.command == 0x16) {
        // forward with pump
        stepControlWithPump(false, 'f');
      
    } else if (IrReceiver.decodedIRData.command == 0xD) {
        // backward with pump
        stepControlWithPump(false, 'b');
      
    } else if (IrReceiver.decodedIRData.command == 0x19) {
        // stop with pump
        stepControlWithPump(true, dirValue);
    }
  }
}

void buttonsListener() {
  int forBut = digitalRead(forwardButton);
  int stopBut = digitalRead(stopButton);
  int backBut = digitalRead(backwardButton);

  if (forBut == 1) {
    stepControlWithoutPump(false, 'f');
  }

  if (stopBut == 1) {
    stepControlWithoutPump(true, 'f');
  }

  if (backBut) {
    stepControlWithoutPump(false, 'b');
  }
}

void limitSensorsListener() {
  int rightLimit = digitalRead(limitButtonRight);
  int leftLimit = digitalRead(limitButtonLeft);

  if (rightLimit == 1 && dirValue == 'f') {
    stepControlWithoutPump(true, 'r');
  }

  if (leftLimit == 1 && dirValue == 'b') {
    stepControlWithoutPump(true, 'l');
  }

  //Serial.print("DIR Value: ");
  //Serial.println(dirValue);
//  Serial.print("Limit Sensor Left: ");
//  Serial.println(leftLimit); 
//  delay(500);
}

/**
 * Sleep Algo
 */
//void goingToSleep() {
//  Serial.println("---going to sleep---");
//  sleep_enable(); // Enabling sleep mode
//  attachInterrupt(digitalPinToInterrupt(interruptPin), wakeUp, LOW); // attaching pin interrupt to pin 2
//  set_sleep_mode(SLEEP_MODE_PWR_DOWN); // Setting the sleep mode(Full sleep)
//  // seet indicator that the system is off (LED turn off)
//  digitalWrite(LED_BUILTIN, LOW);
//  delay(1000); // give time to fully turn off indicator
//  sleep_cpu(); // activating sleep mode
//  Serial.println("Just woke up!"); // Execute code after interrupt
//  // power on indicator
//  digitalWrite(LED_BUILTIN, HIGH);
//}

//void wakeUp() {
//  Serial.println("Interrupt Fired");
//  sleep_disable();
//  detachInterrupt(digitalPinToInterrupt(interruptPin));
//  // setup actions
//}
