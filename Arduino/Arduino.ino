/*
 * 4988 stepper driver gripper tester (Nano) ATmega328p
 * nano, gripper, film pressure sensor, homing switch, open close gipper push button
 */

// ## Libraries ##
/* none yet */

// ########################
// ## PIN CONFIGURATIONS ##
// ########################
int dirPin          = 2;    // 4988 driver direction pin
int stepPin         = 3;    // 4988 driver ste pin
int endStopPin      = 4;    // homing endstop
int enablePin       = 5;    // 4988 driver enable pin
int ms1Pin          = 6;    // 4988 driver microstepping 1 pin
int ms2Pin          = 7;    // 4988 driver microstepping 2 pin
int ms3Pin          = 8;    // 4988 driver microstepping 3 pin
int toggleBtnPin    = 12;   // close|open signal (PC817)
int firmPressurePin = A0;   // Firm pressure strip sensor pin, sensor connected with 1kΩ resistor.


// Variables
const unsigned long stepInterval = 50000UL;
boolean homingDone = false;
boolean gripperOpen = true; // after homing its open
const int stepsPerRevolution = 48;   // steps per revolution
const int gripperStepsFullyOpen = -1500; // how many steps needed to reverse when gripper is fully open
int gripPressureValue = 0;


// SETUP
void setup() {
  // Init serial
  Serial.begin(9600);
  
  // Pin modes
  pinMode(dirPin, OUTPUT);
  pinMode(stepPin, OUTPUT);
  pinMode(enablePin, OUTPUT);
  pinMode(ms1Pin, OUTPUT);
  pinMode(ms2Pin, OUTPUT);
  pinMode(ms3Pin, OUTPUT);
  pinMode(endStopPin, INPUT_PULLUP);
  pinMode(toggleBtnPin, INPUT_PULLUP);
  
  // Enable 1/16 microstepping
  digitalWrite(ms1Pin,HIGH);
  digitalWrite(ms2Pin,HIGH);
  digitalWrite(ms3Pin,HIGH);
  
  // Enable stepper
  digitalWrite(enablePin, HIGH);
}


// LOOP
void loop() {
  if (!homingDone) {
    homing();
  }

  // Read firm pressure sensor (force sensing resistor?)
  gripPressureValue = analogRead(firmPressurePin);

  // Gripper toggle state
  int tInputVal = digitalRead(toggleBtnPin);
  if (tInputVal == LOW) {
    gripperOpen = !gripperOpen;
    delay(1000); // wait so that toggle is not read again multiple times
  }

  // Gripper control
  if (gripperOpen) {
    /* Todo */
  } else {
    /* Todo */
  }
  
}


/**
 * Homing procedure
 * 1. close gripper more till hit estop
 * 2. open gripper fully
 * 3. set homing done
 */
void homing() {
  Serial.println("Homing gripper...");
  int steps = 0;
  int eStopVal = HIGH;
  while (eStopVal == HIGH) {
    eStopVal = digitalRead(endStopPin);
    stepForward();
    steps++;
  }
  
  Serial.print("Gripper endstop hit at ");
  Serial.print(steps);
  Serial.println(" steps.. gripper is fully closed");
  Serial.println("Gripper fully open, saving current position");
  homingDone = true;
}


void stepForward() {
  digitalWrite(dirPin, LOW); // Set the spinning direction clockwise:
  for (int i = 0; i < stepsPerRevolution; i++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(stepInterval);  //1.8 degrees/step
    delay(10);
    digitalWrite(stepPin, LOW);
  }
}
