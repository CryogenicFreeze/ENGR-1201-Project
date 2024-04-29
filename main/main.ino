#include <Adafruit_MotorShield.h>
#include <NewPing.h>
#include <Servo.h>
#include <IRremote.h>

// Photocell ======================================================
const int photocellPin = 0;
const int led_pin_R = 6;
const int led_pin_G = 5;
const int led_pin_B = 3;
int led_brightness;

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
void setup() {
  Serial.begin(9600); //set serial output to 9600 baud
  myMotorShield.begin(); //start the dc motor shield library
  myservo.attach(servoPin); //start servo connections at the assigned pin
  myservo.write(90); // set the servo to forward
  irrecv.enableIRIn(); //setup the ir reciever

  pinMode(led_pin_R, OUTPUT);
  pinMode(led_pin_G, OUTPUT);
  pinMode(led_pin_B, OUTPUT);
}

// main loop ======================================================
void loop() {
  if (is_running == 1){
    //check_infront();
    check_sides();
    led_color(0,255,0);
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

// photocell function =============================================
void led_color(int red, int green, int blue) {
  int photocellReading = 1023 - analogRead(photocellPin);
  led_brightness = map(photocellReading, 0, 1023, 0, 255);
  analogWrite(led_pin_R, red);
  analogWrite(led_pin_G, green); // 0 < val < 255
  analogWrite(led_pin_B, blue);
   if (photocellReading < 300){
    led_brightness = 255;
  } else if (photocellReading > 300){
    led_brightness = 0;
  }
}

// turn servo left or right =======================================
void turn_sensor(int val) {
  if (val == 0) // 0 for 180 degrees 
  {
    myservo.write(180);
  } else if (val == 1) // 1 for 0 degrees
  {
    myservo.write(0);
  }
}

// check for ir remote signal =====================================
void signal_check() {
  if (irrecv.decode(&results)) {
    Serial.println(results.value, HEX);
    if (results.value == 0XFFFFFFFF) { //block external values
      is_running = !is_running; // flip is_running boolean
      Serial.println(is_running);
      delay(100);
    }
    irrecv.resume();
  }
}

// check on either side ===========================================
void check_sides() {
  myservo.write(60);
  delay(250);
  l_wall_dist = get_distance() * (sqrt(3)/2);
  myservo.write(120);
  delay(250);
  r_wall_dist = get_distance() * (sqrt(3)/2);
  if (l_wall_dist < 30 and r_wall_dist == 0){
    turn(0, 225);
  } else if (r_wall_dist < 30 and l_wall_dist == 0){
    turn(1, 255);
  }
}

// check for obstacles ============================================
void check_infront() {
  get_distance();
  if ((distance < 30) and (distance != 0))
  {
    stop(100);
    led_color(255,0,0);
    reroute();
  } else
  {
    forward_backward(0);
    set_speed(100);
    return;
  }
}

// get distance from sensor ========================================
int get_distance() {
  duration = sonar.ping_median(5);
  distance = duration * 0.034/2;
  return distance;
}

// rerouting logic =================================================
void reroute() {
  turn_sensor(0);
  delay(250);
  left_dist = get_distance(); // turn sensor to the left and check the distance 
  delay(250);
  turn_sensor(1);
  delay(250);
  right_dist = get_distance(); // turn sensor to the right and check distance
  delay(250);
  myservo.write(90);
  led_color(255,255,0);
  if (abs(left_dist - right_dist) < 5 and (left_dist != 0) and (right_dist != 0)) { // if the distances are close and not = 0 turn 180
    turn(0, 1070); // turn 180
  } else if (left_dist < right_dist) { // if the distances are non similar and the left is closer turn right
    turn(0, 535); // turn right
  } else {
    turn(1, 535); //turn left
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
void forward_backward(int val){ // 0 for forward, 1 for backward
  if (val == 0){
      Front_Left->run(FORWARD);
      Back_Right->run(FORWARD);
      Front_Right->run(FORWARD);
      Back_Left->run(FORWARD);
    } else if (val == 1){
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
