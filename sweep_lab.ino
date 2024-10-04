#include <Servo.h>
 
int trigPin = 11;    // Trigger
int echoPin = 10;    // Echo
int servoPin = 3;    // Servo
int pos = 0;
int maxDist = 1000;
int minDist = 0;
Servo myservo;  // create servo object to control a servo
long duration, cm, inches;
 
void setup() {
  //Serial Port begin
  Serial.begin (9600);
  myservo.attach(servoPin);  // attaches the servo on pin 9 to the servo object

  //Define inputs and outputs
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}
 
void loop() {
  // The sensor is triggered by a HIGH pulse of 10 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
 
  // Read the signal from the sensor: a HIGH pulse whose
  // duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);
 
  // Convert the time into a distance
  cm = (duration/2) / 29.1;     // Divide by 29.1 or multiply by 0.0343
  inches = (duration/2) / 74;   // Divide by 74 or multiply by 0.0135
  
  Serial.print(inches);
  Serial.print("in, ");
  Serial.print(cm);
  Serial.print("cm");
  Serial.println();
  
  pos = map(cm, minDist, maxDist, 0, 180); 
  
  myservo.write(pos);              // tell servo to go to position in variable 'pos'

  delay(250);
}
