int trigPinL = 12;   // Trigger for left ultrasonic sensor
int echoPinL = 10;   // Echo for left ultrasonic sensor 
int trigPinR = 2;    // Trigger for right ultrasonic sensor
int echoPinR = 3;    // Echo for right ultrasonic sensor
int distL, distR;

void setup() {
  // put your setup code here, to run once:
  // Initialize serial communication
  Serial.begin(9600);

  // Set up ultrasonic sensor pins
  pinMode(trigPinL, OUTPUT);
  pinMode(echoPinL, INPUT);
  pinMode(trigPinR, OUTPUT);
  pinMode(echoPinR, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  measureDistanceL();
  measureDistanceR();
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
  //errorL = SET_POINT - distL;

  // Output the distances to the Serial Monitor
  Serial.print("LEFT: ");
  Serial.print(distL);
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

  // Convert the duration to distance in centimeters
  distR = (duration / 2) / 29.1;
  //errorL = SET_POINT - distL;

  // Output the distances to the Serial Monitor
  Serial.print("\tRIGHT: ");
  Serial.print(distR);
  Serial.println(" cm");

}
