// Photocell RGB =============================================================
int photocellPin = 0;     // the cell and 10K pulldown are connected to a0
int photocellReading;     // the analog reading from the sensor divider
int LEDpinR = 6;          // connect Red LED to pin 6 (PWM pin)
int LEDpinG = 5;          // connect Green LED to pin 5 (PWN pin)

int LEDpinB = 3;          // connect Blue LED to pin 3 (PWN pin)
int LEDbrightness;        // 
void setup(void) {
  // We'll send debugging information via the Serial monitor
  Serial.begin(9600);   
}
 
void loop() 
{
  photocellReading = analogRead(photocellPin);  
 
  Serial.print("Analog reading = ");
  Serial.println(photocellReading);     // the raw analog reading
    if (photocellReading < 300) {
    Serial.println(" - Dark");
  } else if (photocellReading > 300) {
    Serial.println(" - Light");
  }
  
  LEDbrightness = map(photocellReading, 0, 1023, 0, 255);
      if (photocellReading < 300) {
        LEDbrightness = 255;
    } else if (photocellReading > 300) {
        LEDbrightness = 0;
    }

  analogWrite(LEDpinR, LEDbrightness);
  analogWrite(LEDpinB, LEDbrightness);
  delay(1000);
  analogWrite(LEDpinB, LOW);
  analogWrite(LEDpinG, LEDbrightness);
  delay(1000);
  analogWrite(LEDpinR, LOW);
  delay(1000);
  analogWrite(LEDpinG, LOW);
}