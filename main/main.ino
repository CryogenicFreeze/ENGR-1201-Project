#include <Adafruit_MotorShield.h>
Adafruit_MotorShield myMotorShield = Adafruit_MotorShield();

Adafruit_DCMotor *Front_Left = myMotorShield.getMotor(1); //M1 port
Adafruit_DCMotor *Back_Right = myMotorShield.getMotor(2); //M2 port
Adafruit_DCMotor *Front_Right = myMotorShield.getMotor(3); //M3 port
Adafruit_DCMotor *Back_Left = myMotorShield.getMotor(4); //M4 port

void setup()
{
  myMotorShield.begin();
}

void loop()
{
  forward_backward(0);
  set_speed(accel_ease(0.1, 100, 250));
}

int accel_ease(float dx, int wait_time, int max_speed) //declare dx step value and time between loops in ns, and max motor speed(less than 255)
{
  float y;
  for (float x = 0; x <= 1.5; x += dx) 
  {
    y = max_speed*(pow(x, 2) - (pow(x, 4)/3) + ((2*pow(x, 6))/45) - (pow(x, 8)/315)); //easing function
    return y;
    delay(wait_time);
  }
}

void stop()
{
  Front_Left->run(RELEASE);
  Back_Left->run(RELEASE);
  Front_Right->run(RELEASE);
  Back_Right->run(RELEASE);
}

void set_speed(int speed) // max 255
{
  Front_Left->setSpeed(speed);
  Back_Left->setSpeed(speed);
  Front_Right->setSpeed(speed);
  Back_Right->setSpeed(speed);
 }

void forward_backward(int val){ // 0 for forward, 1 for backward
  if (val = 0)
    {
      Front_Left->run(FORWARD);
      Back_Right->run(FORWARD);
      Front_Right->run(FORWARD);
      Back_Left->run(FORWARD);
    } else if (val = 1)
    {
      Front_Left->run(BACKWARD);
      Back_Right->run(BACKWARD);
      Front_Right->run(BACKWARD);
      Back_Left->run(BACKWARD);
    }
 }

void left_right(int val) // 0 for right turn, 1 for left turn
{ 
  if (val = 0)
  {
      Front_Left->run(FORWARD);
      Back_Left->run(FORWARD);
      Front_Right->run(BACKWARD);
      Back_Right->run(BACKWARD);
  } else if (val = 1)
  {
      Front_Left->run(BACKWARD);
      Back_Right->run(FORWARD);
      Front_Right->run(FORWARD);
      Back_Left->run(BACKWARD);
  }
}
