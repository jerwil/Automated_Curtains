// Simple Motor Shield sketch

#include <Servo.h>


const int button1Pin = 3;     // the number of the pushbutton pin
const int button2Pin = 4;     // the number of the 2nd pushbutton pin

int button1State = 0; 
int button2State = 0; 

int motor_state = 0;


// Declare classes for Servo connectors of the MotorShield.
Servo servo_1;
Servo servo_2;


void setup()
{
  Serial.begin(9600);
  Serial.println("Simple Motor Shield sketch");
  pinMode(button1Pin, INPUT);
  pinMode(button2Pin, INPUT);

  // Use the default "Servo" library of Arduino.
  // Attach the pin number to the servo library.
  // This might also set the servo in the middle position.
  servo_1.attach(9);
  servo_2.attach(10);
}


void loop()
{

  button1State = digitalRead(button1Pin);
  button2State = digitalRead(button2Pin);
  // Suppose there are two servo motors connected.
  // Let them move 180 degrees.


//  servo_2.write(0);
//  delay(1000);
//  servo_2.write(180);
//  delay(2000);


if (button1State == HIGH && button2State == LOW) {
  servo_1.write(180);
  Serial.println("Forward!");
  }
else if(button2State == HIGH && button1State == LOW){
  servo_1.write(0);
  Serial.println("Backward!");
}
else{
  servo_1.write(90);
}
}



