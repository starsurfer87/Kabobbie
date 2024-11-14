// Ultrasonic sensor and motor control pins
int trigPinF = 12;   // Trigger for left ultrasonic sensor
int echoPinF = 10;   // Echo for left ultrasonic sensor 
int trigPinR = 2;    // Trigger for right ultrasonic sensor
int echoPinR = 3;    // Echo for right ultrasonic sensor

// Motor control pins
int motorAPin_A = 8; // Arduino digital 8 connected to HG7881's A-1A terminal
int motorAPin_B = 9; // Arduino digital 9 connected to HG7881's A-1B terminal
int motorBPin_A = 4; // Arduino digital 4 connected to HG7881's B-1A terminal
int motorBPin_B = 5; // Arduino digital 5 connected to HG7881's B-1B terminal

// Distance variables
int distF, errorF;
int SET_POINT = 10;  // Stop the car if an obstacle is closer than this distance (cm)

// Minimum power required for motion
int BASE = 120;

void setup() {
  // Initialize serial communication
  Serial.begin(9600);

  // Set up ultrasonic sensor pins
  pinMode(trigPinF, OUTPUT);
  pinMode(echoPinF, INPUT);
  pinMode(trigPinR, OUTPUT);
  pinMode(echoPinR, INPUT);

  // Set up motor pins as outputs
  pinMode(motorAPin_A, OUTPUT);
  pinMode(motorAPin_B, OUTPUT);
  pinMode(motorBPin_A, OUTPUT);
  pinMode(motorBPin_B, OUTPUT);
}

void loop() {
  // Measure the distance
  measureDistanceF();

  // If no obstacle within STOP_DISTANCE, move forward; otherwise, stop
  if (errorF > 300) {
    setRightMotor(-100);
    setLeftMotor(-100);
  } else if (errorF < -300) {
    setRightMotor(100);
    setLeftMotor(100);
  } else if (abs(errorF) < 2) {
    setRightMotor(0);
    setLeftMotor(0);
  } else {
    setRightMotor(errorF*(-100)/300);
    setLeftMotor(errorF*(-100)/300);
  }

  delay(250);  // Wait a bit before the next reading
}

// Measure distance using the ultrasonic sensor
void measureDistanceF() {
  // Send a 10-microsecond pulse to trigger the ultrasonic sensor
  digitalWrite(trigPinF, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPinF, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPinF, LOW);

  // Measure the echo signal duration
  long duration = pulseIn(echoPinF, HIGH);

  // Convert the duration to distance in centimeters
  distF = (duration / 2) / 29.1;
  errorF = SET_POINT - distF;

  // Output the distance to the Serial Monitor
  Serial.print(distF);
  Serial.println(" cm");
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

// Inverts value for backward motion
int invert(int input) {
  return 255 - input;
}
