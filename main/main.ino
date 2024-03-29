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
  check_infront();
  Serial.println(get_distance());
  if (irrecv.decode(&results)) {
    stop(10000);
		irrecv.resume(); // Receive the next value 
	}
	delay (100); // small delay to prevent reading errors
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

// check for obstacles ============================================
void check_infront()
{
  get_distance();
  if ((distance < 30) and (distance != 0))
  {
    stop(1000);
    reroute();
  } else
  {
    forward_backward(0);
    set_speed(125);
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

// rerouting logic =================================================
void reroute()
{
  turn_sensor(0);
  delay(600);
  left_dist = get_distance(); // turn sensor to the left and check the distance 
  delay(600);
  turn_sensor(1);
  delay(600);
  right_dist = get_distance(); // turn sensor to the right and check distance
  if (abs(left_dist - right_dist) < 5 and (left_dist != 0) and (right_dist != 0)) // if the distances are close and not = 0 turn 180
  {
    //turn 180
  } else if (left_dist < right_dist) // if the distances are non similar and the left is closer turn right
  {
    //turn right
  } else 
  {
    //turn left
  }
}

// easing function ================================================
int accel_ease(float dx, int wait_time, int max_speed) //declare dx step value and time between loops in ns, and max motor speed(less than 255)
{
  float y;
  for (float x = 0; x <= 1.5; x += dx) 
  {
    y = max_speed*(pow(x, 2) - (pow(x, 4)/3) + ((2*pow(x, 6))/45) - (pow(x, 8)/315)); //easing function
    delay(wait_time);
  }
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
