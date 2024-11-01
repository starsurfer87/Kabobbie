// Motor control pins
// A is left wheel, B is right wheel 
const int motorAPin_A = 8; // Arduino digital 8 connected to HG7881's A-1A terminal
const int motorAPin_B = 9; // Arduino digital 9 connected to HG7881's A-1B terminal
const int motorBPin_A = 4; // Arduino digital 4 connected to HG7881's B-1A terminal
const int motorBPin_B = 5; // Arduino digital 5 connected to HG7881's B-1B terminal

// Motor constants
const int POWER = 10; // standard power output to motors, must be in [-100,100]
const int BASE = 120; // Minimum power required for motion

// Photocell variables and constants
int photocellValL = 0;
int photocellValR = 0;
const int photocellPinL = A0;
const int photocellPinR = A2;
const int THRESHOLD = 550; 

// History
enum Motion {
  STRAIGHT,
  LEFT,
  RIGHT
};
Motion prevChoice = STRAIGHT;

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

  if (!onLine(photocellValL) && !onLine(photocellValR)) {
    setRightMotor(POWER);
    setLeftMotor(POWER);
    prevChoice = STRAIGHT;
    Serial.println("\tGO STRAIGHT");
  } else if (!onLine(photocellValL) && onLine(photocellValR) ) {
    setRightMotor(0);
    setLeftMotor(POWER);
    prevChoice = RIGHT;
    Serial.println("\tTURN RIGHT");
  } else if (onLine(photocellValL) && !onLine(photocellValR)) {
    setRightMotor(POWER);
    setLeftMotor(0);
    prevChoice = LEFT;
    Serial.println("\tTURN LEFT");
  } else {
    if (prevChoice == STRAIGHT) {
      setRightMotor(0);
      setLeftMotor(0);
      Serial.println("\tPANIC");
    } else if (prevChoice == RIGHT) {
      setRightMotor(0);
      setLeftMotor(POWER-5);
      Serial.println("\tTENTATIVE RIGHT");
    } else if (prevChoice == LEFT) {
      setRightMotor(POWER-5);
      setLeftMotor(0);
      Serial.println("\tTENTATIVE LEFT");
    }
  }

  delay(250);  // Wait a bit before the next reading
}

// read values from right and left photocellss
void readPhotocells() {
  
  photocellValL = analogRead(photocellPinL);
  photocellValR = analogRead(photocellPinR);
  
  Serial.print("left: ");
  Serial.print(photocellValL);
  Serial.print("\tright: ");
  Serial.print(photocellValR);
}

// returns true if phorocell detects line, false otherwise
bool onLine(int photocellVal) {
  return photocellVal > THRESHOLD;
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
