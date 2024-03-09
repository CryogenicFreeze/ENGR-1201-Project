#include <Adafruit_MotorShield.h>
#include <NewPing.h>
#include <Servo.h>

// setup ultrasonic sensor ========================================
const int trigPin = 9; // trigger pin set to 9
const int echoPin = 10; // echo pin set to 10
long duration;
int distance;
NewPing sonar(trigPin, echoPin, 200);

//setup servo =====================================================
const int servoPin = 11; // servo pwr set to pin 11
Servo servo;

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
  myMotorShield.begin();
}

// main loop ======================================================
void loop()
{
  forward_backward(0);
  set_speed(accel_ease(0.1, 100, 250));
  check_infront();
}

// turn servo left or right =======================================
void servo_turn(int val)
{
  if (val = 0)
  {
    servo.write(45);
  } else if (val = 1)
  {
    servo.write(90);
  }
}

// check for obstacles ============================================
void check_infront()
{
  duration = sonar.ping_median(5);
  if (duration = 0) // error protection for 0 duration
  {
    duration = 1824; // set duration to 1824 microseconds = 31cm
  }
  distance = duration * 0.034/2;
  Serial.print("Distance: ");
  Serial.println(distance);
  if ((distance < 30) and (distance != 0))
  {
    stop();
    // run reroute routine here
  } else
  {
    // drive forward
    return;
  }
}

// easing function ================================================
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

// function to stop car =========================================
void stop()
{
  Front_Left->run(RELEASE);
  Back_Left->run(RELEASE);
  Front_Right->run(RELEASE);
  Back_Right->run(RELEASE);
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

// function to turn ============================================
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
