#include <Adafruit_MotorShield.h>
#include <NewPing.h>
#include <Servo.h>
#include <IRremote.h>

// ir sensor ======================================================
const int irPin = 7;
IRrecv irrecv(irPin);
decode_results results;

// setup ultrasonic sensor ========================================
const int trigPin = 11; // trigger pin set to 9
const int echoPin = 10; // echo pin set to 10
long duration;
int distance;
float left_dist;
float right_dist;
NewPing sonar(trigPin, echoPin, 200); // initialize the sonar sensor and set max distance to 200cm

//setup servo =====================================================
const int servoPin = 9; // servo pwr set to pin 11
Servo myservo;

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
  myservo.attach(servoPin); //start servo connections at the assigned pin
  myservo.write(90); // set the servo to forward
  irrecv.enableIRIn(); // start the ir reciever
}

// main loop ======================================================
void loop()
{
  forward_backward(0);
  set_speed(100);
  check_infront();
}

void one_meter(float time)
{
  forward_backward(0);
  set_speed(100);
  delay(time);
  stop(5000);
}

// turn servo left or right =======================================
void turn_sensor(int val)
{
  if (val == 0) // 0 for 180 degrees 
  {
    myservo.write(180);
  } else if (val == 1) // 1 for 0 degrees
  {
    myservo.write(0);
  }
}

void turn_right(float time)
{
  left_right(0);
  set_speed(100);
  delay(time);
}

// check for obstacles ============================================
void check_infront()
{
  get_distance();
  if ((distance < 35) and (distance != 0))
  {
    stop(1000);
    turn_right(535);
    one_meter(3175);
    stop(5000);
  } else
  {
    //forward_backward(0);
    //set_speed(150);
    return;
  }
}

// get distance from sensor ========================================
int get_distance()
{
  duration = sonar.ping_median(5);
  distance = duration * 0.034/2;
  return distance;
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
