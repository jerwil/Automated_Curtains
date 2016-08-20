// Simple Motor Shield sketch

#include <Servo.h>

//Timekeeping variables:

double milis_timer[1] = {0}; // This is used to keep track of the timer used to tick for each milisecond
double second_timer[1] = {0}; // This is used to keep track of the timer used to tick for each second
unsigned long currentTime;

double open_time = 7; // Time in seconds to open curtains
double close_time = 7; // Time in seconds to reset string

double motor_timer = 0;

// Button Variables

const int button1Pin = 3;     // the number of the pushbutton pin
const int button2Pin = 4;     // the number of the 2nd pushbutton pin

int button1State = 0; 
int button2State = 0; 

int button1_press_initiate[1];     // storage for button press function
int button1_press_completed[1];    // storage for button press function
int button1_pushed;

int button2_press_initiate[1];     // storage for button press function
int button2_press_completed[1];    // storage for button press function
int button2_pushed;


int motor_state = 0;

char* mode = "idle";



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
    button1_pushed = button_press(button1State, button1_press_initiate, button1_press_completed);
    button2_pushed = button_press(button2State, button2_press_initiate, button2_press_completed);
  
  if (mode == "idle"){
  motor_state = 0;
  if (button1_pushed == 1) {
    mode = "open";
    Serial.println("Open");
    button1_pushed = 0;
    }
  else if(button2_pushed == 1){
    mode = "close";
    Serial.println("Close");
    button2_pushed = 0;
  }
  else {
    mode = "idle";
  }
  }
  
  else if (mode == "open"){
    if (tick(250,second_timer) == 1){
      motor_timer += .25;
      Serial.print("Motor timer: ");
      Serial.print(motor_timer);
      Serial.print("/");
      Serial.println(open_time);
    }
    if (motor_timer >= open_time || button1_pushed == 1 || button2_pushed == 1){
      mode = "idle";
      motor_timer = 0;
      Serial.println("Idle State");
      button1_pushed = 0;
      button2_pushed = 0;
    }
    motor_state = 1;
  }
  else if (mode == "close"){
    if (tick(250,second_timer) == 1){
      motor_timer += .25;
      Serial.print("Motor timer: ");
      Serial.print(motor_timer);
      Serial.print("/");
      Serial.println(close_time);
    }
    if (motor_timer >= close_time || button1_pushed == 1 || button2_pushed == 1){
      mode = "idle";
      motor_timer = 0;
      Serial.println("Idle State");
      button1_pushed = 0;
      button2_pushed = 0;
    }
    motor_state = -1;
  }
  
  
  // Write to motor
  if (motor_state == 1){
    servo_1.write(180);
  }
  else if (motor_state == -1){
    servo_1.write(0);
  }
  else if (motor_state == 0){
    servo_1.write(90);
  }
  else{
    servo_1.write(90);  
  }

}

int tick(int delay, double timekeeper[1]){
currentTime = millis();
if(currentTime >= (timekeeper[0] + delay)){
  timekeeper[0] = currentTime;
  return 1;
  }
else {return 0;}
}

int button_press (int button_indicator, int button_press_initiated[1], int button_press_complete[1]){
  if (button_indicator == 0 && button_press_initiated[0] == 1) {
  button_press_complete[0] = 1;
  button_press_initiated[0] = 0;
  }
  else if (button_indicator == 1){
  button_press_initiated[0] = 1;
  button_press_complete[0] = 0;
  }
  else {button_press_complete[0] = 0;}
return button_press_complete[0];
}

