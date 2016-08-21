// Simple Motor Shield sketch

#include <Servo.h>

//Timekeeping variables:

double milis_timer[1] = {0}; // This is used to keep track of the timer used to tick for each milisecond
double second_timer[1] = {0}; // This is used to keep track of the timer used to tick for each second
double buffer_timer[1] = {0}; // This is used to keep track of the timer used to tick for each second
unsigned long currentTime;
word alarm_time_buffer = 7200; // If the alarm has gone off, wait until this amount of seconds have passed before it can be triggered again.
word alarm_time_passed = 7200; // The amount of seconds since the alarm was triggered
int pause_time = 10; // Time in seconds to pause between opening and closing
int pause_counter = 0; // Counter to track how long the system has paused for

double open_time = 9; // Time in seconds to open curtains
double close_time = 8.2; // Time in seconds to reset string

double motor_timer = 0;

// Button Variables

const int button1Pin = 3;     // the number of the pushbutton pin
const int button2Pin = 4;     // the number of the 2nd pushbutton pin
const int DIPin = 12;

int button1State = 0; 
int button2State = 0; 
int alarm_signal = 0;
int alarm_signal_timer = 0; // Timer and target are used to make sure that alarm signal is high for at least the target time to avoid noise
int alarm_signal_target = 2;



int motor_state = 0;

int curtain_state = 0; // Curtain is closed when state is 0, open when 1

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
  pinMode(DIPin, INPUT);

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
    alarm_signal = digitalRead(DIPin);
    
  if (tick(1000,buffer_timer) == 1){
    alarm_time_passed += 1;
    if (alarm_time_passed > 32000){alarm_time_passed = alarm_time_buffer + 1;} // Avoid overflow
    if (alarm_signal_timer >= alarm_signal_target && alarm_time_passed < alarm_time_buffer){
      Serial.println("Alarm is sounding, but it's too soon!");
      Serial.print(alarm_time_passed);
      Serial.print("/");
      Serial.print(alarm_time_buffer);
      Serial.println(" seconds since alarm went off");
    }
  }
  if (mode == "idle"){
    if (button1State == HIGH && button2State == LOW) {
      motor_state = 1;
      Serial.println("Forward!");
    }
    else if(button2State == HIGH && button1State == LOW){
      motor_state = -1;
      Serial.println("Backward!");
    }
    else{
      motor_state = 0;
    }
  if (tick(1000,second_timer) == 1){
  if (alarm_signal == 1) {    
      alarm_signal_timer +=1;
  }
  else {alarm_signal_timer = 0;}
  }
    if (alarm_signal_timer >= alarm_signal_target && alarm_time_passed > alarm_time_buffer){
    mode = "open";
    Serial.println("Open");
    alarm_signal_timer = 0;
    }
    
  }
  
  else if (mode == "open"){
    if (tick(100,milis_timer) == 1){
      motor_timer += .10;
      Serial.print("Motor timer: ");
      Serial.print(motor_timer);
      Serial.print("/");
      Serial.println(open_time);
    }
    if (motor_timer >= open_time){ 
      mode = "pause";
      motor_timer = 0;
      Serial.println("Pausing");
    }
    else if (button1State == HIGH || button2State == HIGH){ // Allow user to interrupt automatic opening
      mode = "idle";
      motor_timer = 0;
      Serial.println("interrupted by user");
    }
    motor_state = 1;
    pause_counter = 0;
    alarm_time_passed = 0;
  }
  
  else if (mode == "pause"){
    motor_state = 0;
    if (tick(1000,milis_timer) == 1){
      pause_counter += 1;
      Serial.print("Pause timer: ");
      Serial.print(pause_counter);
      Serial.print("/");
      Serial.println(pause_time);
    }
    if (pause_counter >= pause_time){
      mode = "close";
      pause_counter = 0;
    }
  }
  
  else if (mode == "close"){
    if (tick(100,milis_timer) == 1){
      motor_timer += .10;
      Serial.print("Motor timer: ");
      Serial.print(motor_timer);
      Serial.print("/");
      Serial.println(close_time);
    }
    if (motor_timer >= close_time || button1State == HIGH || button2State == HIGH){
      mode = "idle";
      motor_timer = 0;
      Serial.println("Idle State");
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

