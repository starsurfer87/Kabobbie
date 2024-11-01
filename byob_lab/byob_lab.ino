/**
 * Create variables to be used to run motor A
 */
int motorAPin_A = 8; //Arduino digital 8 is connected to HG7881's A-1A terminal
int motorAPin_B = 9; //Arduino digital 9 is connected to HG7881's A-1B terminal

int motorBPin_A = 4; //Arduino digital 4 is connected to HG7881's B-1A terminal
int motorBPin_B = 5; //Arduino digital 5 is connected to HG7881's B-1B terminal

int BASE = 120;

void setup(){
  /**
   * When program starts set Arduino pinmode for 8 and 9 digital to be OUTPUT
   * so we can use analogWrite to output values from 0 to 255 (0-5V) (PWM) 
   */
  
  // Initialize serial communication
  Serial.begin(9600);
  
  pinMode(motorAPin_A, OUTPUT); // A direction
  pinMode(motorAPin_B, OUTPUT); // A speed
  
  pinMode(motorBPin_A, OUTPUT); // B direction
  pinMode(motorBPin_B, OUTPUT); // B speed

}

void loop() {
  forward();
  delay(2000);

  stop();
  delay(1000);

  backward();
  delay(2000);

  stop();
  delay(1000);

  turnRight();
  delay(1000);

  stop();
  delay(1000);

  turnLeft();
  delay(1000);

  stop();
  delay(1000);
}


int invertOurValue(int input) {
  return 255 - input;
}

void forward() {
  // set direction 
  digitalWrite(motorAPin_A, LOW);
  digitalWrite(motorBPin_A, LOW);
  Serial.println("FORWARD");

  // increase speed
  for(int i= BASE; i<=255; i++){
    analogWrite(motorAPin_B, i);
    analogWrite(motorBPin_B, i);
    delay(40);
  }
}

void backward() {
  // set direction
  digitalWrite(motorAPin_A, HIGH);
  digitalWrite(motorBPin_A, HIGH);
  Serial.println("BACK");

  // increase speed
  for(int i=BASE; i<=255; i++){
    analogWrite(motorAPin_B, invertOurValue( i ) );
    analogWrite(motorBPin_B, invertOurValue( i ) );
    delay(40);
  }
}

void turnLeft() {
  // set direction 
  digitalWrite(motorAPin_A, HIGH);
  digitalWrite(motorBPin_A, LOW);
  digitalWrite(motorAPin_B, HIGH);
  Serial.println("LEFT");

  // increase speed
  for(int i= BASE; i<=255; i++){
    analogWrite(motorBPin_B, i);
    delay(40);
  }
}

void turnRight() {
   // set direction 
  digitalWrite(motorAPin_A, LOW);
  digitalWrite(motorBPin_A, HIGH);
  digitalWrite(motorBPin_B, HIGH);
  Serial.println("RIGHT");

  // increase speed
  for(int i= BASE; i<=255; i++){
    analogWrite(motorAPin_B, i);
    delay(40);
  }
}

void stop() {
  digitalWrite(motorAPin_A, HIGH);
  digitalWrite(motorAPin_B, HIGH);
  digitalWrite(motorBPin_A, HIGH);
  digitalWrite(motorBPin_B, HIGH);
}

