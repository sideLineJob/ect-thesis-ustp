
const int stepPinTop = 2;
const int dirPinTop = 3;
const int stepPinDown = 4;
const int dirPinDown = 5;

int customDelayMapped; // Defines variables

int counter = 0;
char dir = 'f';
boolean stopStep = false;
 
void setup() {
  Serial.begin(115200);
  // Sets the two pins as Outputs
  pinMode(stepPinTop, OUTPUT);
  pinMode(stepPinDown, OUTPUT);
  pinMode(dirPinTop, OUTPUT);
  pinMode(dirPinDown, OUTPUT);

  stepMove(dir);
}
void loop() {

  counter++;

  if (counter == 1000) {
    counter = 0;
//    Serial.println("\n\n ---Rotate--- \n\n");
//    stopStep = stopStep ? false : true;
//    if (stopStep)
    

    if (dir == 'f')
      dir = 'l';
    else
      dir = 'f';

    stepMove(dir);
      
    delay(2000);
//    Serial.print("Direction: ");
//    Serial.println(dir);
  }

  if (!stopStep) {
    runStepper();
  }
  
  Serial.print("Speed: ");
  Serial.println(customDelayMapped);
}
// Function for reading the Potentiometer
int speedUp() {
  int customDelay = analogRead(A0); // Reads the potentiometer
  int newCustom = map(customDelay, 0, 1023, 200, 800); // default(300,4000) stable(50, 400) Convrests the read values of the potentiometer from 0 to 1023 into desireded delay values (300 to 4000)
  return newCustom;  
}

void runStepper() {
  customDelayMapped = speedUp(); // Gets custom delay values from the custom speedUp function
  
  digitalWrite(stepPinTop, HIGH);
  digitalWrite(stepPinDown, HIGH);
  delayMicroseconds(customDelayMapped);
  digitalWrite(stepPinTop, LOW);
  digitalWrite(stepPinDown, LOW);
  delayMicroseconds(customDelayMapped);
}

void stepMove(char dir) {
  if (dir == 'f' || dir == 'F') {
    // Forward direction
    digitalWrite(dirPinTop, HIGH);
    digitalWrite(dirPinDown, LOW);
  } else {
    digitalWrite(dirPinTop, LOW);
    digitalWrite(dirPinDown, HIGH);
  }
}
