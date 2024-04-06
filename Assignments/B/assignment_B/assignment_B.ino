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
  set_speed(0);
}

// main loop ======================================================
void loop()
{
  one_meter(3175);
  turn_left(535);
  one_meter(3175);
  stop(5000);
}

void one_meter(float time)
{
  forward_backward(0);
  set_speed(100);
  delay(time);
  stop(100);
}

void turn_left(float time)
{
  left_right(1);
  set_speed(100);
  delay(time);
}

// function to stop car =========================================
void stop(float time)
{
  Front_Left->run(RELEASE);
  Back_Left->run(RELEASE);
  Front_Right->run(RELEASE);
  Back_Right->run(RELEASE);
  delay(time);
}

// function to set car speed ====================================
void set_speed(int speed) // max 255
{
  Front_Left->setSpeed(speed+8);
  Back_Left->setSpeed(speed+8);
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

// function to turn ============================================
void left_right(int val) // 0 for right turn, 1 for left turn
{ 
  if (val == 0)
  {
      Front_Left->run(FORWARD);
      Back_Left->run(FORWARD);
      Front_Right->run(BACKWARD);
      Back_Right->run(BACKWARD);
  } else if (val == 1)
  {
      Front_Left->run(BACKWARD);
      Back_Right->run(FORWARD);
      Front_Right->run(FORWARD);
      Back_Left->run(BACKWARD);
  }
}
