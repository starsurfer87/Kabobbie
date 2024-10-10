// Ultrasonic sensor and motor control pins
int trigPin = 11;    // Trigger
int echoPin = 10;    // Echo

// Motor control pins
int motorAPin_A = 8; // Arduino digital 8 connected to HG7881's A-1A terminal
int motorAPin_B = 9; // Arduino digital 9 connected to HG7881's A-1B terminal
int motorBPin_A = 4; // Arduino digital 4 connected to HG7881's B-1A terminal
int motorBPin_B = 5; // Arduino digital 5 connected to HG7881's B-1B terminal

// Distance variables
int duration, dist, error;
int SET_POINT = 20;  // Stop the car if an obstacle is closer than this distance (cm)

// Minimum power required for motion
int BASE = 120;

void setup() {
  // Initialize serial communication
  Serial.begin(9600);

  // Set up ultrasonic sensor pins
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Set up motor pins as outputs
  pinMode(motorAPin_A, OUTPUT);
  pinMode(motorAPin_B, OUTPUT);
  pinMode(motorBPin_A, OUTPUT);
  pinMode(motorBPin_B, OUTPUT);
}

void loop() {
  // Measure the distance
  measureDistance();

  // If no obstacle within STOP_DISTANCE, move forward; otherwise, stop
  if (error > 300) {
    powerOutput(-100);
  } else if (error < -300) {
    powerOutput(100);
  } else if (abs(error) < 2) {
    powerOutput(0);
  } else {
    powerOutput(error*(-100)/300);
  }

  delay(250);  // Wait a bit before the next reading
}

// Measure distance using the ultrasonic sensor
void measureDistance() {
  // Send a 10-microsecond pulse to trigger the ultrasonic sensor
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Measure the echo signal duration
  duration = pulseIn(echoPin, HIGH);

  // Convert the duration to distance in centimeters
  dist = (duration / 2) / 29.1;
  error = SET_POINT - dist;

  // Output the distance to the Serial Monitor
  Serial.print(dist);
  Serial.println(" cm");
}

// Move car based on power output
void powerOutput(long output) {
  if (output == 0) {
    stop();
  } else if (output > 0) {
    int outputMapped = map(output, 0, 100, BASE, 255);
    forward(outputMapped);
  } else {
    int outputMapped = map(output, 0, -100, BASE, 255);
    backward(outputMapped);
  }
}

// Move the car forward
void forward(int power) {
  // Set direction forward
  analogWrite(motorAPin_A, LOW);
  analogWrite(motorBPin_A, LOW);

  analogWrite(motorAPin_B, power);
  analogWrite(motorBPin_B, power);
}

// Move car backward
void backward(int power) {
  // Set direction backward
  analogWrite(motorAPin_A, 255);
  analogWrite(motorBPin_A, 255);

  analogWrite(motorAPin_B, power);
  analogWrite(motorBPin_B, power);
}

// Stops motors
void stop() {
  analogWrite(motorAPin_A, LOW);
  analogWrite(motorAPin_B, LOW);
  analogWrite(motorBPin_A, LOW);
  analogWrite(motorBPin_B, LOW);
}

// Inverts value for backward motion
int invertOurValue(int input) {
  return 255 - input;
}
