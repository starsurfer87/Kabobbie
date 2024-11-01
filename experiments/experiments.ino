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
int photocellValL = 0;
int photocellValR = 0;

const int photocellPinL = A0;
const int photocellPinR = A2;

const int THRESHOLD = 710; 

int photocellValL_last = 0;
int photocellValR_last = 0;


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

    // setRightMotor(POWER);
    // setLeftMotor(POWER);

  if (!onLine(photocellValL) && !onLine(photocellValR)) {
    straight();
    //setRightMotor(POWER);
    //setLeftMotor(POWER);
    Serial.print("GO STRAIGHT");
  } else if (!onLine(photocellValL) && onLine(photocellValR)) {
    right();
    //setRightMotor(0);
    //setLeftMotor(POWER);
    Serial.print("TURN RIGHT");
  } else if (onLine(photocellValL) && !onLine(photocellValR)) {
    left();
    //setRightMotor(POWER);
    // setLeftMotor(0);
    Serial.print("TURN LEFT");
  } else {
    stop();
    // setRightMotor(0);
    // setLeftMotor(0);
    Serial.print("PANIC!!!!!!!!!!");
  }

  delay(250);  // Wait a bit before the next reading
}

// read values from right and left photocellss
void readPhotocells() {
  
  photocellValL = analogRead(photocellPinL);
  photocellValR = analogRead(photocellPinR);

  
  int photocellValL_avg = (photocellValL + photocellValL_last) / 2;
  int photocellValR_avg = (photocellValR + photocellValR_last) / 2;

  photocellValL_last = photocellValL_avg;
  photocellValR_last = photocellValR_avg;
  
  Serial.print("\t\tleft: ");
  Serial.print(photocellValL);
  Serial.print("\tright: ");
  Serial.print(photocellValR);
  
  Serial.print("\tleft avg: ");
  Serial.print(photocellValL_avg);
  Serial.print("\tright avt: ");
  Serial.println(photocellValR_avg);


}

// returns true if phorocell detects line, false otherwise
bool onLine(int photocellVal) {
  return photocellVal > THRESHOLD;
}
// takes a value between [-100, 100] and set power output of right motor accordingly
void setRightMotor(int val) {
  digitalWrite(motorBPin_B, HIGH);
  digitalWrite(motorBPin_A, HIGH);
}

// takes a value between [-100, 100] and set power output of left motor accordingly
void setLeftMotor(int val) {
  digitalWrite(motorAPin_B, HIGH);
  digitalWrite(motorAPin_A, HIGH);
}

void left() {
  analogWrite(motorAPin_B, HIGH);
  digitalWrite(motorAPin_A, LOW);

  analogWrite(motorBPin_B, HIGH);
  digitalWrite(motorBPin_A, HIGH);
}

void straight() {
  analogWrite(motorAPin_B, HIGH);
  digitalWrite(motorAPin_A, LOW);

  analogWrite(motorBPin_B, HIGH);
  digitalWrite(motorBPin_A, LOW);

}


void right() {
  analogWrite(motorAPin_B, HIGH);
  digitalWrite(motorAPin_A, HIGH);

  analogWrite(motorBPin_B, HIGH);
  digitalWrite(motorBPin_A, LOW);
}


void stop() {
  digitalWrite(motorAPin_B, HIGH);
  digitalWrite(motorAPin_A, HIGH);

  digitalWrite(motorBPin_B, HIGH);
  digitalWrite(motorBPin_A, HIGH);
}
