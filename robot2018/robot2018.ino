#include <AFMotor.h>
#include <Servo.h>

// PIN's
#define RIGHT_MOTOR_PIN 3
#define LEFT_MOTOR_PIN 4
#define NECK_PIN 10
#define EYE_TRIGGER_PIN 14
#define EYE_ECHO_PIN 15
#define BT_PIN_RX 16
#define BT_PIN_TX 17

// Loop Vars
int NCicles = 0;

// Eye Vars
int EYE_CONTACT = 30;
int distance = 0;

// Neck Vars
Servo neck;
int MIN_NECK_ANGLE = 30;
int MAX_NECK_ANGLE = 150;
int NECK_PACE = 20;
int neckrotation;
int angle;

// DC Motors vars
int MOTORS_CALIBRATION_OFFSET = 0;
int MOTORS_MAXSPEED = 100;
double MOTOR_LEFT_FORCE = 1;
double MOTOR_RIGHT_FORCE = 1;
AF_DCMotor rightMotor(RIGHT_MOTOR_PIN,MOTOR12_64KHZ); 
AF_DCMotor leftMotor(LEFT_MOTOR_PIN,MOTOR12_64KHZ); 
String motorSet = "FORWARD";

// TourVars
//DXXX[TT] -- Se Deteta a menos de XXX então executa tour TT
//RXXX[TT] -- Se Repete XXX vezes      então executa tour TT

// Tours
String tour = "00";
// 00 - Idle             00                     --> R010[01]
// 01 - Scan             01,02,10               --> D050[03] R10[02] 
// 02 - Avoid <--        01,02,03               --> D010[03] R10[01]
// 03 - Chace            01,02,04               --> D010[04] R5[02]
// 04 - Attack           02,05,06               --> D010[04] D050[03] R10[01]

// Program Blocks
// 00 - Idle
// 01 - Rotate Neck , if not rotate move to 90º
// 02 - Read Eye
// 03 - Avoid
// 04 - Chase
// 05 - Scan Attack (b,f,l,r,r,l,f,b)
// 06 - Switch on Tazer
// 07 - Switch off Tazer
// 08 - Up arm
// 09 - Down arm
// 10 - Rotate 90º
// 11 - Rotate 180º

// Blynk Event
//void myTimerEvent()
//{
//  // You can send any value at any time.
//  // Please don't send more that 10 values per second.
//  Blynk.virtualWrite(V5, millis() / 1000);
//}


void setup() {
  
  // Serial Setup
  Serial.begin(9600); // Starts the serial communication
  Serial.println("Serial connection setup [DONE]");

  // BlueTooth
  // SerialBLE.begin(9600);
  
  // Blynk
  // Blynk.begin(SerialBLE, auth);
  // timer.setInterval(1000L, myTimerEvent);

  // Neck Setup
  neck.attach(NECK_PIN);
  neck.write(90);
  neckrotation = -1;
  delay(2000);

  //Motor Setup
  MOTOR_LEFT_FORCE = 1;
  MOTOR_RIGHT_FORCE = 1;
  leftMotor.setSpeed(MOTOR_LEFT_FORCE * MOTORS_MAXSPEED - (MOTORS_CALIBRATION_OFFSET / 2)); 
  rightMotor.setSpeed(MOTOR_RIGHT_FORCE * MOTORS_MAXSPEED + (MOTORS_CALIBRATION_OFFSET / 2)); 
  moveForward();
  //Eye Setup
  pinMode(EYE_TRIGGER_PIN, OUTPUT); // Sets the EYE_TRIGGER_PIN as an Output
  pinMode(EYE_ECHO_PIN, INPUT);  // Sets the EYE_ECHO_PIN as an Input

  // Tour Setup
  tour = "00";

  
}

void loop() {
  NCicles++; 
  // rotate
  rotateNeck();
  
  // read sensors
  readEye();  
  
  //tour 
  if(tour = "00") { };  // idle
  if(tour = "01") { };  // scan
  if(tour = "02") { };  // avoid
  if(tour = "03") { };  // chase  
  if(tour = "03") { };  // attack
}

// **************
// PROGRAM BLOCKS
// **************

void rotateNeck() {
  if(angle <= MIN_NECK_ANGLE) {
    neckrotation = +1;
  } 
  if(angle >= MAX_NECK_ANGLE){
    neckrotation = -1;
  }
  angle = angle + neckrotation * NECK_PACE;
  neck.write(angle);
  delay(45);
  
}

void readEye() {
  int duration = 0;
  // Clears the EYE_TRIGGER_PIN
  digitalWrite(EYE_TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  // Sets the EYE_TRIGGER_PIN on HIGH state for 10 micro seconds
  digitalWrite(EYE_TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(EYE_TRIGGER_PIN, LOW);
  // Reads the EYE_ECHO_PIN, returns the sound wave travel time in microseconds
  duration = pulseIn(EYE_ECHO_PIN, HIGH);
  // Calculating the distance
  distance = duration * 0.034/2; 
}

void Avoid() {
  if(distance <= EYE_CONTACT) {
    if(angle<90){
      turnRight(100);
    } else {
      turnLeft(100);
    }
  }
}

void Chase() {
  if(distance <= EYE_CONTACT) {
    if(angle<90){
      turnLeft(100);
    } else {
      turnRight(100);
    }
  }
}



void moveStop() {
  leftMotor.run(RELEASE);  // stop the motors.
  rightMotor.run(RELEASE);
}

void moveForward() {
  motorSet = "FORWARD";
  leftMotor.run(FORWARD);      // turn it on going forward
  rightMotor.run(FORWARD);      // turn it on going forward
}

void moveBackward() {
  motorSet = "BACKWARD";
  leftMotor.run(BACKWARD);      // turn it on going forward
  rightMotor.run(BACKWARD);     // turn it on going forward
}

void turnRight(int tt) {
  motorSet = "RIGHT";
  leftMotor.run(FORWARD);      // turn motor 1 forward
  rightMotor.run(BACKWARD);     // turn motor 2 backward
  delay(tt); // run motors this way for tt ms
  moveForward();
}

void turnLeft(int tt) {
  motorSet = "LEFT";
  leftMotor.run(BACKWARD);     // turn motor 1 backward
  rightMotor.run(FORWARD);      // turn motor 2 forward
  delay(tt); // run motors this way for tt ms
  moveForward();
}

void rotare90() {
  motorSet = "LEFT";
  leftMotor.run(BACKWARD);     // turn motor 1 backward
  rightMotor.run(FORWARD);      // turn motor 2 forward
  delay(50); // run motors this way for tt ms
  moveForward();
}

void rotare180() {
  motorSet = "LEFT";
  leftMotor.run(BACKWARD);     // turn motor 1 backward
  rightMotor.run(FORWARD);      // turn motor 2 forward
  delay(50); // run motors this way for tt ms
  moveForward();
}
