// Ultrasonic sensor and motor control pins
int trigPinL = 11;   // Trigger for left ultrasonic sensor
int echoPinL = 12;   // Echo for left ultrasonic sensor
int trigPinR = 3;    // Trigger for right ultrasonic sensor
int echoPinR = 6;    // Echo for right ultrasonic sensor

// Motor control pins
// A is right wheel, B is left wheel
int motorAPin_A = 8; // Arduino digital 8 connected to HG7881's A-1A terminal
int motorAPin_B = 9; // Arduino digital 9 connected to HG7881's A-1B terminal
int motorBPin_A = 4; // Arduino digital 4 connected to HG7881's B-1A terminal
int motorBPin_B = 5; // Arduino digital 5 connected to HG7881's B-1B terminal

// Distance variables
int distL, distR;
//int SET_POINT = 15;  // Stop the car if an obstacle is closer than this distance (cm)
int BUFF = 2; // allowed difference between distL and distR, must be a positive integer -- CAN FINE TUNE
int POWER = 15; // standard power output to motors, should be in [-100,100] -- CAN FINE TUNE

// Minimum power required for motion
int BASE = 120;

void setup() {
  // Initialize serial communication
  Serial.begin(9600);

  // Set up ultrasonic sensor pins
  pinMode(trigPinL, OUTPUT);
  pinMode(echoPinL, INPUT);

  // Set up motor pins as outputs
  pinMode(motorAPin_A, OUTPUT);
  pinMode(motorAPin_B, OUTPUT);
  pinMode(motorBPin_A, OUTPUT);
  pinMode(motorBPin_B, OUTPUT);
}

void loop() {
  measureDistanceR();
  measureDistanceL();

  if (abs(distL-distR) < BUFF) {
    setRightMotor(POWER);
    setLeftMotor(POWER);
    Serial.println("GO STRAIGHT");
  } else if (distR > distL) {
    setRightMotor(0);
    setLeftMotor(POWER);
    Serial.println("TURN RIGHT");
  } else if (distR < distL) {
    setRightMotor(POWER);
    setLeftMotor(0);
    Serial.println("TURN LEFT");
  }


  delay(250);  // Wait a bit before the next reading

}

// takes a value between [-100, 100] and set power output of right motor accordingly
void setRightMotor(int val) {
  if (val == 0) {
    analogWrite(motorAPin_B, LOW);
  } else if (val > 0) {
    int outputMapped = map(val, 0, 100, BASE, 255);
    analogWrite(motorAPin_A, LOW);
    analogWrite(motorAPin_B, outputMapped);
  } else {
    int outputMapped = map(val, 0, -100, BASE, 255);
    analogWrite(motorAPin_A, 255);
    analogWrite(motorAPin_B, invert(outputMapped));
  }
}

// takes a value between [-100, 100] and set power output of left motor accordingly
void setLeftMotor(int val) {
  if (val == 0) {
    analogWrite(motorBPin_B, LOW);
  } else if (val > 0) {
    int outputMapped = map(val, 0, 100, BASE, 255);
    analogWrite(motorBPin_A, LOW);
    analogWrite(motorBPin_B, outputMapped);
  } else {
    int outputMapped = map(val, 0, -100, BASE, 255);
    analogWrite(motorBPin_A, 255);
    analogWrite(motorBPin_B, invert(outputMapped));
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
  int duration = pulseIn(echoPinL, HIGH);

  // Convert the duration to distance in centimeters
  distL = (duration / 2) / 29.1;
  //errorL = SET_POINT - distL;

  // Output the distances to the Serial Monitor
  Serial.print("LEFT: ");
  Serial.print(distL);
  Serial.println(" cm");



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
  int duration = pulseIn(echoPinR, HIGH);

  // Convert the duration to distance in centimeters
  distR = (duration / 2) / 29.1;
  //errorR = SET_POINT - distR;

  // Output the distance to the Serial Monitor
  Serial.print("RIGHT: ");
  Serial.print(distR);
  Serial.println(" cm");
}

// Inverts value for backward motion
int invert(int input) {
  return 255 - input;
}
