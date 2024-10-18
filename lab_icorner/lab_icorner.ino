// Ultrasonic sensor and motor control pins
int trigPinL = 11;   // Trigger for left ultrasonic sensor
int echoPinL = 10;   // Echo for left ultrasonic sensor 
int trigPinR = 3;    // Trigger for right ultrasonic sensor
int echoPinR = 6;    // Echo for right ultrasonic sensor

// Motor control pins
// A is left wheel, B is right wheel 
int motorAPin_A = 8; // Arduino digital 8 connected to HG7881's A-1A terminal
int motorAPin_B = 9; // Arduino digital 9 connected to HG7881's A-1B terminal
int motorBPin_A = 4; // Arduino digital 4 connected to HG7881's B-1A terminal
int motorBPin_B = 5; // Arduino digital 5 connected to HG7881's B-1B terminal

// Distance variables
int distL, distR, errorL;
int SET_POINT = 17;  // Stop the car if an obstacle is closer than this distance (cm)
int POWER = 15; // standard power output to motors, should be in [-100,100] -- CAN FINE TUNE

// Minimum power required for motion
int BASE = 120;

// Directions
int IN = 0;
int OUT = 1;
int dir;

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

  measureDistanceL();
  Serial.print(errorL);
  Serial.println();

  if (errorL > 0 && dir == IN) {
    dir = OUT;
    Serial.println("Change to Out");
  } else if (errorL < -3 && dir == OUT) {
    dir = IN;
    Serial.println("Change to In");
  }

  // Measure the distance
  if (dir == IN) {
    setRightMotor(POWER);
    setLeftMotor(0);
    Serial.println("Moving In");
  } else if (dir == OUT) {
    setRightMotor(0);
    setLeftMotor(POWER); 
    Serial.println("Moving Out");
  }

  delay(250);  // Wait a bit before the next reading
}

// takes a value between [-100, 100] and set power output of right motor accordingly
void setRightMotor(int val) {
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

// takes a value between [-100, 100] and set power output of left motor accordingly
void setLeftMotor(int val) {
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

  // Convert the duration to distance in centimeters
  distL = (duration / 2) / 29.1;
  errorL = SET_POINT - distL;

  // Output the distances to the Serial Monitor
  //Serial.print("LEFT: ");
  //Serial.print(distL);
  //Serial.println(" cm");



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

// // Ultrasonic sensor and motor control pins
// int trigPin = 11;    // Trigger
// int echoPin = 10;    // Echo

// // Motor control pins
// // A is left wheel, B is right wheel
// int motorAPin_A = 8; // Arduino digital 8 connected to HG7881's A-1A terminal
// int motorAPin_B = 9; // Arduino digital 9 connected to HG7881's A-1B terminal
// int motorBPin_A = 4; // Arduino digital 4 connected to HG7881's B-1A terminal
// int motorBPin_B = 5; // Arduino digital 5 connected to HG7881's B-1B terminal

// // Distance variables
// int dist, error;
// int SET_POINT = 15;  // Stop the car if an obstacle is closer than this distance (cm)

// // Minimum power required for motion
// int BASE = 120;

// void setup() {
//   // Initialize serial communication
//   Serial.begin(9600);

//   // Set up ultrasonic sensor pins
//   pinMode(trigPin, OUTPUT);
//   pinMode(echoPin, INPUT);

//   // Set up motor pins as outputs
//   pinMode(motorAPin_A, OUTPUT);
//   pinMode(motorAPin_B, OUTPUT);
//   pinMode(motorBPin_A, OUTPUT);
//   pinMode(motorBPin_B, OUTPUT);
// }

// void loop() {
//   // Measure the distance
//   while (error < 0) {
//     setRightMotor(15);
//     setLeftMotor(0);
//     measureDistance();
//     Serial.print(error);
//     Serial.println();
//     delay(250);  // Wait a bit before the next reading
//   } 
//   while (error > -3) {
//     setRightMotor(0);
//     setLeftMotor(15);
//     measureDistance();
//     Serial.print(error);
//     Serial.println();
//     delay(250);  // Wait a bit before the next reading
//   }
// }

// // takes a value between [-100, 100] and set power output of right motor accordingly
// void setRightMotor(int val) {
//   if (val == 0) {
//     analogWrite(motorBPin_B, LOW);
//   } else if (val > 0) {
//     int outputMapped = map(val, 0, 100, BASE, 255);
//     analogWrite(motorBPin_A, LOW);
//     analogWrite(motorBPin_B, outputMapped);
//   } else {
//     int outputMapped = map(val, 0, -100, BASE, 255);
//     analogWrite(motorBPin_A, 255);
//     analogWrite(motorBPin_B, invert(outputMapped));
//   }
// }

// // takes a value between [-100, 100] and set power output of left motor accordingly
// void setLeftMotor(int val) {
//   if (val == 0) {
//     analogWrite(motorAPin_B, LOW);
//   } else if (val > 0) {
//     int outputMapped = map(val, 0, 100, BASE, 255);
//     analogWrite(motorAPin_A, LOW);
//     analogWrite(motorAPin_B, outputMapped);
//   } else {
//     int outputMapped = map(val, 0, -100, BASE, 255);
//     analogWrite(motorAPin_A, 255);
//     analogWrite(motorAPin_B, invert(outputMapped));
//   }
// }

// // Measure distance using the ultrasonic sensor
// void measureDistance() {
//   // Send a 10-microsecond pulse to trigger the ultrasonic sensor
//   digitalWrite(trigPin, LOW);
//   delayMicroseconds(5);
//   digitalWrite(trigPin, HIGH);
//   delayMicroseconds(10);
//   digitalWrite(trigPin, LOW);

//   // Measure the echo signal duration
//   long duration = pulseIn(echoPin, HIGH);

//   // Convert the duration to distance in centimeters
//   dist = (duration / 2) / 29.1;
//   error = SET_POINT - dist;

//   // Output the distance to the Serial Monitor
//   Serial.print(dist);
//   Serial.println(" cm");
// }

// // Inverts value for backward motion
// int invert(int input) {
//   return 255 - input;
// }
