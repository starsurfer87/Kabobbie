// Ultrasonic sensor and motor control pins
int trigPinL = 12;   // Trigger for left ultrasonic sensor
int echoPinL = 10;   // Echo for left ultrasonic sensor 
int trigPinR = 2;    // Trigger for right ultrasonic sensor
int echoPinR = 3;    // Echo for right ultrasonic sensor

// Motor control pins
// A is left wheel, B is right wheel 
int motorAPin_A = 8; // Arduino digital 8 connected to HG7881's A-1A terminal
int motorAPin_B = 9; // Arduino digital 9 connected to HG7881's A-1B terminal
int motorBPin_A = 4; // Arduino digital 4 connected to HG7881's B-1A terminal
int motorBPin_B = 5; // Arduino digital 5 connected to HG7881's B-1B terminal

// Distance variables
int currDistL, currDistR, prevDistL, prevDistR, filteredDistL, filteredDistR;
int errorL, errorR;
int SET_POINT = 7;  // Stop the car if an obstacle is closer than this distance (cm)
int BUFF = 2; // allowed difference between distL and distR, must be a positive integer -- CAN FINE TUNE
int POWER = 15; // standard power output to motors, should be in [-100,100] -- CAN FINE TUNE
float ALPHA = 1;

// Minimum power required for motion
int BASE = 120;

enum Direction {
  IN,
  OUT
};
Direction dir = IN;

void setup() {
  // Initialize serial communication
  Serial.begin(9600);

  // Set up ultrasonic sensor pins
  pinMode(trigPinL, OUTPUT);
  pinMode(echoPinL, INPUT);
  pinMode(trigPinR, OUTPUT);
  pinMode(echoPinR, INPUT);

  // Set up motor pins as outputs
  pinMode(motorAPin_A, OUTPUT);
  pinMode(motorAPin_B, OUTPUT);
  pinMode(motorBPin_A, OUTPUT);
  pinMode(motorBPin_B, OUTPUT);

  // Take some initial sensor readings
  filteredDistL = -1;
  filteredDistR = -1;

  for (int i=0; i<3; i++) {
    measureDistanceL();
    Serial.println();
  }
  Serial.println("START");
}

void loop() {
  measureDistanceR();

  if (errorR < 0) {
    setRightMotor(0);
    setLeftMotor(POWER);
    Serial.println("\tMoving In");
  } else {
    setRightMotor(POWER);
    setLeftMotor(0);
    Serial.println("\tMoving Out");
  }

  delay(250);  // Wait a bit before the next reading

}

// takes a value between [-100, 100] and set power output of right motor accordingly
void setRightMotor(int val) {
  if (val == 0) {
    digitalWrite(motorBPin_A, HIGH);
    digitalWrite(motorBPin_B, HIGH);
  } else if (val > 0) {
    int outputMapped = map(val, 0, 100, BASE, 255);
    digitalWrite(motorBPin_A, LOW);
    analogWrite(motorBPin_B, outputMapped);
  } else {
    int outputMapped = map(-val, 0, 100, BASE, 255);
    digitalWrite(motorBPin_A, HIGH);
    analogWrite(motorBPin_B, invert(outputMapped));
  }
}

// takes a value between [-100, 100] and set power output of left motor accordingly
void setLeftMotor(int val) {
  if (val == 0) {
    digitalWrite(motorAPin_A, HIGH);
    digitalWrite(motorAPin_B, HIGH);
  } else if (val > 0) {
    int outputMapped = map(val, 0, 100, BASE, 255);
    digitalWrite(motorAPin_A, LOW);
    analogWrite(motorAPin_B, outputMapped);
  } else {
    int outputMapped = map(-val, 0, 100, BASE, 255);
    digitalWrite(motorAPin_A, HIGH);
    analogWrite(motorAPin_B, invert(outputMapped));
  }
}

// Measure distance using the ultrasonic sensor
void measureDistanceL() {
  // Send a 10-microsecond pulse to trigger the ultrasonic sensor
  digitalWrite(trigPinL, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPinL, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPinL, LOW);

  // Measure the echo signal duration
  long duration = pulseIn(echoPinL, HIGH);

  // Calculate distance based on new measurement and prev value
  prevDistL = filteredDistL;
  currDistL = (duration / 2) / 29.1;
  if (prevDistL >= 0) {
    filteredDistL = ALPHA*currDistL + (1-ALPHA)*prevDistL;
  } else {
    filteredDistL = currDistL;
    Serial.print("TEST");
  }
  errorL = SET_POINT - filteredDistL;

  // Output the distances to the Serial Monitor
  Serial.print("Current Left: ");
  Serial.print(currDistL);
  Serial.print(" cm");

  Serial.print("\tFiltered Left: ");
  Serial.print(filteredDistL);
  Serial.print(" cm");

  Serial.print("\tPrevious Left: ");
  Serial.print(prevDistL);
  Serial.print(" cm");

  Serial.print("\tLeft Error: ");
  Serial.print(errorL);
  Serial.print(" cm");
}

// Measure distance using the ultrasonic sensor
void measureDistanceR() {
  // Send a 10-microsecond pulse to trigger the ultrasonic sensor
  digitalWrite(trigPinR, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPinR, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPinR, LOW);

  // Measure the echo signal duration
  long duration = pulseIn(echoPinR, HIGH);

  // Calculate distance based on new measurement and prev value
  prevDistR = filteredDistR;
  currDistR = (duration / 2) / 29.1;
  if (prevDistR >= 0) {
    filteredDistR = ALPHA*currDistR + (1-ALPHA)*prevDistR;
  } else {
    filteredDistR = currDistR;
    Serial.print("TEST");
  }
  errorR = SET_POINT - filteredDistR;

  // Output the distances to the Serial Monitor
  Serial.print("Current Right: ");
  Serial.print(currDistR);
  Serial.print(" cm");

  Serial.print("\tFiltered Right: ");
  Serial.print(filteredDistR);
  Serial.print(" cm");

  Serial.print("\tPrevious Right: ");
  Serial.print(prevDistR);
  Serial.print(" cm");

  Serial.print("\tRight Error: ");
  Serial.print(errorR);
  Serial.print(" cm");
}

// Measure distance using the ultrasonic sensor
// void measureDistanceR() {
//   // Send a 10-microsecond pulse to trigger the ultrasonic sensor
//   digitalWrite(trigPinR, LOW);
//   delayMicroseconds(5);
//   digitalWrite(trigPinR, HIGH);
//   delayMicroseconds(10);
//   digitalWrite(trigPinR, LOW);

//   // Measure the echo signal duration
//   long duration = pulseIn(echoPinR, HIGH);

//   // Convert the duration to distance in centimeters
//   distR = (duration / 2) / 29.1;
//   //errorR = SET_POINT - distR;

//   // Output the distance to the Serial Monitor
//   Serial.print("\tRIGHT: ");
//   Serial.print(distR);
//   Serial.print(" cm");
// }

// Inverts value for backward motion
int invert(int input) {
  return 255 - input;
}

