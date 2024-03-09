
#include <Adafruit_MotorShield.h>
Adafruit_MotorShield myMotorShield = Adafruit_MotorShield();

Adafruit_DCMotor *Left_F = myMotorShield.getMotor(1); //left front
Adafruit_DCMotor *Right_B = myMotorShield.getMotor(2); //right back
Adafruit_DCMotor *Right_F = myMotorShield.getMotor(3); //right front
Adafruit_DCMotor *Left_B = myMotorShield.getMotor(4); //left back

void setup() {
  myMotorShield.begin();
}

void loop() {
  direction(0);
  set_speed(125);
}

void set_speed(int speed){
  Left_F->setSpeed(speed);
  Left_B->setSpeed(speed);
  Right_F->setSpeed(speed);
  Right_B->setSpeed(speed);
 }

void direction(int val){ // forward val = 0 backward val = 1
  if (val = 0){
      Left_F->run(FORWARD);
      Right_B->run(FORWARD);
      Right_F->run(FORWARD);
      Left_B->run(FORWARD);
    } else if (val = 1){
      Left_F->run(BACKWARD);
      Right_B->run(BACKWARD);
      Right_F->run(BACKWARD);
      Left_B->run(BACKWARD);
      }
 }

void turn(int l_r){ //turn right l_r=0 turn left l_r=1
  if (l_r = 0){
      Left_F->run(FORWARD);
      Right_B->run(BACKWARD);
      Right_F->run(BACKWARD);
      Left_B->run(FORWARD);
    } else if (l_r = 1) {
      Left_F->run(BACKWARD);
      Right_B->run(FORWARD);
      Right_F->run(FORWARD);
      Left_B->run(BACKWARD);
      }
  }
