// Motor control pins
// A is left wheel, B is right wheel 
const int motorAPin_A = 8; // Arduino digital 8 connected to HG7881's A-1A terminal
const int motorAPin_B = 9; // Arduino digital 9 connected to HG7881's A-1B terminal
const int motorBPin_A = 4; // Arduino digital 4 connected to HG7881's B-1A terminal
const int motorBPin_B = 5; // Arduino digital 5 connected to HG7881's B-1B terminal

// Motor constants
const int POWER = 15; // standard power output to motors, must be in [-100,100]
const int BASE = 120; // Minimum power required for motion

// Photocell variables and constants
int photocellValR = 0;
int photocellValL = 0;
const int photocellPinL = A0;
const int photocellPinR = A1;
const int THRESHOLD = 700; 

void setup() {
  // Initialize serial communication
  Serial.begin(9600);

  // Set up motor pins as outputs
  pinMode(motorAPin_A, OUTPUT);
  pinMode(motorAPin_B, OUTPUT);
  pinMode(motorBPin_A, OUTPUT);
  pinMode(motorBPin_B, OUTPUT);
}

void loop() {
  readPhotocells();

  if (!onLine(photocellValR) && !onLine(photocellValL)) {
    setRightMotor(POWER);
    setLeftMotor(POWER);
    //Serial.println("GO STRAIGHT");
  } else if (onLine(photocellValR) && !onLine(photocellValL)) {
    setRightMotor(0);
    setLeftMotor(POWER);
    //Serial.println("TURN RIGHT");
  } else if (!onLine(photocellValR) && onLine(photocellValL)) {
    setRightMotor(POWER);
    setLeftMotor(0);
    //Serial.println("TURN LEFT");
  } else {
    setRightMotor(0);
    setLeftMotor(0);
    //Serial.println("PANIC");
  }

  delay(250);  // Wait a bit before the next reading
}

// read values from right and left photocellss
void readPhotocells() {
  photocellValR = analogRead(photocellPinR);
  photocellValL = analogRead(photocellPinL);
  Serial.print("right: ");
  Serial.println(photocellValR);
  Serial.print("left: ");
  Serial.println(photocellValL);
}

// returns true if phorocell detects line, false otherwise
bool onLine(int photocellVal) {
  return photocellVal > THRESHOLD;
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

// Inverts value for backward motion
int invert(int input) {
  return 255 - input;
}
