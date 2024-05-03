#include <Adafruit_MotorShield.h>
#include <NewPing.h>
#include <Servo.h>
#include <IRremote.h>

// leds ======================================================
const int led_pin_R = 5;
const int led_pin_Y = 4;
const int led_pin_G = 3;

// ir sensor ======================================================
const int irPin = 7;
IRrecv irrecv(irPin);
decode_results results;
bool is_running = 0;

// setup ultrasonic sensor ========================================
const int trigPin = 11; // trigger pin set to 9
const int echoPin = 10; // echo pin set to 10
long duration;
int distance;
float left_dist;
float right_dist;
float l_wall_dist;
float r_wall_dist;
float f_dist;
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

// init ===========================================================
void setup() {
  Serial.begin(9600); //set serial output to 9600 baud
  myMotorShield.begin(); //start the dc motor shield library
  myservo.attach(servoPin); //start servo connections at the assigned pin
  myservo.write(90); // set the servo to forward
  irrecv.enableIRIn(); //setup the ir reciever

  pinMode(led_pin_R, OUTPUT);
  pinMode(led_pin_Y, OUTPUT);
  pinMode(led_pin_G, OUTPUT);
}

// main loop ======================================================
void loop() {
  if (is_running == 1){
    check_for_obstacles();
  } else {
    halt();
  }
  signal_check();
}

// turning function ================================================
void turn(int v, float time) {
  left_right(v);
  set_speed(100);
  delay(time);
}

// check for ir remote signal =====================================
void signal_check() {
  if (irrecv.decode(&results)) {
    if (results.value == 0XFFFFFFFF) { //block external values
      is_running = !is_running; // flip is_running boolean
      delay(100);
    }
    irrecv.resume();
  }
}

void check_if_close(float dist) {
  if (dist < 40 and dist != 0) {
    stop(100);
    digitalWrite(led_pin_R, HIGH);
    digitalWrite(led_pin_G, LOW);
    digitalWrite(led_pin_Y, LOW);
    reroute();
  } else {
    forward_backward(0);
    digitalWrite(led_pin_G, HIGH);
    digitalWrite(led_pin_R, LOW);
    digitalWrite(led_pin_Y, LOW);
    set_speed(100);
  }
}

void check_for_obstacles() {

  myservo.write(150);
  delay(100);
  l_wall_dist = get_distance();
  check_if_close(l_wall_dist);

  myservo.write(90);
  delay(100);
  f_dist = get_distance();
  check_if_close(f_dist);

  myservo.write(30);
  delay(100);
  r_wall_dist = get_distance();
  check_if_close(r_wall_dist);

  myservo.write(90);
  delay(100);
  f_dist = get_distance();
  check_if_close(f_dist);

}

// get distance from sensor ========================================
float get_distance() {
  duration = sonar.ping_median(5);
  distance = duration * 0.034/2;
  Serial.print(distance);
  return distance;
  }

// rerouting logic =================================================
void reroute() {
  myservo.write(180);
  delay(250);
  left_dist = get_distance(); // turn sensor to the left and check the distance 
  delay(250);
  myservo.write(0);
  delay(250);
  right_dist = get_distance(); // turn sensor to the right and check distance
  delay(250);
  myservo.write(90);
  digitalWrite(led_pin_Y, HIGH);
  digitalWrite(led_pin_G, LOW);
  digitalWrite(led_pin_R, LOW);
  if (abs(left_dist - right_dist) < 5 and (left_dist != 0) and (right_dist != 0)) { // if the distances are close and not = 0 turn 180
    turn(0, 1070); // turn 180
  } else if (left_dist < right_dist) { // if the distances are non similar and the left is closer turn right
    turn(0, 435); // turn right
  } else {
    turn(1, 435); //turn left
  }
}

// function to stop car =========================================
void stop(float time) {
  Front_Left->run(RELEASE);
  Back_Left->run(RELEASE);
  Front_Right->run(RELEASE);
  Back_Right->run(RELEASE);
  delay(time);
  }

// completely stop car ==========================================
void halt() {
  Front_Left->run(RELEASE);
  Back_Left->run(RELEASE);
  Front_Right->run(RELEASE);
  Back_Right->run(RELEASE);
  }

// function to set car speed ====================================
void set_speed(int speed) { // max 255
  Front_Left->setSpeed(speed+8);
  Back_Left->setSpeed(speed+8);
  Front_Right->setSpeed(speed);
  Back_Right->setSpeed(speed);
  }

// function to set drive direction to forward or backward =======
void forward_backward(int val) { // 0 for forward, 1 for backward
  if (val == 0){
      Front_Left->run(FORWARD);
      Back_Right->run(FORWARD);
      Front_Right->run(FORWARD);
      Back_Left->run(FORWARD);
    } else if (val == 1) {
      Front_Left->run(BACKWARD);
      Back_Right->run(BACKWARD);
      Front_Right->run(BACKWARD);
      Back_Left->run(BACKWARD);
    }
 }

// function to turn ============================================
void left_right(int val) { // 0 for right turn, 1 for left turn
  if (val == 0) {
      Front_Left->run(FORWARD);
      Back_Left->run(FORWARD);
      Front_Right->run(BACKWARD);
      Back_Right->run(BACKWARD);
    } else if (val == 1) {
      Front_Left->run(BACKWARD);
      Back_Right->run(FORWARD);
      Front_Right->run(FORWARD);
      Back_Left->run(BACKWARD);
    }
  }
