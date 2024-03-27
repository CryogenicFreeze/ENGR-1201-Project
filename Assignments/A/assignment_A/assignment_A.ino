#include <Adafruit_MotorShield.h>

// Setup dc motors ================================================
Adafruit_MotorShield myMotorShield = Adafruit_MotorShield();
Adafruit_DCMotor *Front_Left = myMotorShield.getMotor(1); //M1 port
Adafruit_DCMotor *Back_Right = myMotorShield.getMotor(2); //M2 port
Adafruit_DCMotor *Front_Right = myMotorShield.getMotor(3); //M3 port
Adafruit_DCMotor *Back_Left = myMotorShield.getMotor(4); //M4 port

// init setup =====================================================
void setup()
{
  Serial.begin(9600); //set serial output to 9600 baud
  myMotorShield.begin(); //start the dc motor shield library
}

// main loop ======================================================
void loop()
{
  forward_backward(0);
  set_speed(150);
}

// function to set car speed ====================================
void set_speed(int speed) // max 255
{
  Front_Left->setSpeed(speed);
  Back_Left->setSpeed(speed);
  Front_Right->setSpeed(speed);
  Back_Right->setSpeed(speed);
 }

// function to set drive direction to forward or backward =======
void forward_backward(int val){ // 0 for forward, 1 for backward
  if (val == 0)
    {
      Front_Left->run(FORWARD);
      Back_Right->run(FORWARD);
      Front_Right->run(FORWARD);
      Back_Left->run(FORWARD);
    } else if (val == 1)
    {
      Front_Left->run(BACKWARD);
      Back_Right->run(BACKWARD);
      Front_Right->run(BACKWARD);
      Back_Left->run(BACKWARD);
    }
 }