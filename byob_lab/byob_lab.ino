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
  analogWrite(motorAPin_A, LOW);
  analogWrite(motorBPin_A, LOW);

  // increase speed
  for(int i= BASE; i<=255; i++){
    analogWrite(motorAPin_B, i);
    analogWrite(motorBPin_B, i);
    delay(40);
  }
}

void backward() {
  // set direction
  analogWrite(motorAPin_A, 255);
  analogWrite(motorBPin_A, 255);

  // increase speed
  for(int i=BASE; i<=255; i++){
    analogWrite(motorAPin_B, invertOurValue( i ) );
    analogWrite(motorBPin_B, invertOurValue( i ) );
    delay(40);
  }
}

void turnLeft() {
  // set direction 
  analogWrite(motorAPin_A, LOW);
  analogWrite(motorBPin_A, LOW);
  analogWrite(motorAPin_B, LOW);

  // increase speed
  for(int i= BASE; i<=255; i++){
    analogWrite(motorBPin_B, i);
    delay(40);
  }
}

void turnRight() {
   // set direction 
  analogWrite(motorAPin_A, LOW);
  analogWrite(motorBPin_A, LOW);
  analogWrite(motorBPin_B, LOW);

  // increase speed
  for(int i= BASE; i<=255; i++){
    analogWrite(motorAPin_B, i);
    delay(40);
  }
}

void stop() {
  analogWrite(motorAPin_A, LOW);
  analogWrite(motorAPin_B, LOW);
  analogWrite(motorBPin_A, LOW);
  analogWrite(motorBPin_B, LOW);
}

