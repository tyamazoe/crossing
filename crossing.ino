//
// Crossing
// 2018-08-21 rev 0.1 initial
// 2018-08-30 rev 0.2 mp3 working
// 2018-09-01 rev 0.3 LED blinker
// 2018-09-04 rev 0.4 add mute
//
#include <M5Stack.h>
#include <Ultrasonic.h>

#define ULTRASONIC_PIN  (22)
#define INTERVAL        (300)
#define SERVO_PIN       (5)
#define LED1_PIN        (16)
#define LED2_PIN        (17)

Ultrasonic UltrasonicRanger(ULTRASONIC_PIN);
bool blinker = false;
int currentAngle = 0;
bool beepMute = true;

// Speaker
#define NOTE_D2 330
#define NOTE_D3 350

int servoPulse(int angleDegrees)
{
  int pulseWidth = map(angleDegrees, 0, 180, 544, 2400);
  return pulseWidth;
}

void servoSet(int oldAngle, int newAngle) 
{
  int pulseWidth;
  if (oldAngle == newAngle) { 
    return;
  }
  else if (oldAngle < newAngle) {
    for (int i = oldAngle; i <= newAngle; i+=3){
      pulseWidth = servoPulse(i);
      digitalWrite(SERVO_PIN, HIGH);
      delayMicroseconds(pulseWidth);                          
      digitalWrite(SERVO_PIN, LOW);
      delayMicroseconds(20000 - pulseWidth);   
    }
  }
  else if (oldAngle > newAngle) {
    for (int i = oldAngle; i >= newAngle; i--){
      pulseWidth = servoPulse(i);
      digitalWrite(SERVO_PIN, HIGH);
      delayMicroseconds(pulseWidth);                           
      digitalWrite(SERVO_PIN, LOW);
      delayMicroseconds(20000 - pulseWidth);
    }
  }
}

void crossingClose()
{
  M5.Lcd.fillCircle(100, 100, 50, RED);
  M5.Lcd.fillCircle(240, 100, 50, RED);
  digitalWrite(LED1_PIN, HIGH);
  digitalWrite(LED2_PIN, HIGH);
  servoSet(currentAngle, 90);
  currentAngle = 90;
}

void crossingOpen()
{
  delay(1000);
  servoSet(currentAngle, 0);
  currentAngle = 0;
  M5.Lcd.fillCircle(100, 100, 50, BLACK);
  M5.Lcd.fillCircle(240, 100, 50, BLACK);
  digitalWrite(LED1_PIN, LOW);
  digitalWrite(LED2_PIN, LOW);
}

void crossingBlink(bool blinker) {
  if (blinker) {
    M5.Lcd.fillCircle(100, 100, 50, RED);
    M5.Lcd.fillCircle(240, 100, 50, BLACK);
    if (!beepMute) M5.Speaker.tone(NOTE_D3, 10);
    digitalWrite(LED1_PIN, HIGH);
    digitalWrite(LED2_PIN, LOW);
  }
  else {
    M5.Lcd.fillCircle(100, 100, 50, BLACK);
    M5.Lcd.fillCircle(240, 100, 50, RED);
    M5.Speaker.mute();
    digitalWrite(LED1_PIN, LOW);
    digitalWrite(LED2_PIN, HIGH);
  }
}

void setup() 
{
  M5.begin();
  delay(500);
  M5.Speaker.setVolume(0);
  
  pinMode(SERVO_PIN, OUTPUT);
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  digitalWrite(LED1_PIN, LOW);
  digitalWrite(LED2_PIN, LOW);
  M5.Lcd.fillCircle(100, 100, 60, TFT_DARKGREY);
  M5.Lcd.fillCircle(240, 100, 60, TFT_DARKGREY);
  delay(500);
  //servoSet(45, currentAngle);
  delay(500);
}

void loop()
{
  long distance;
  distance = UltrasonicRanger.MeasureInCentimeters();
  //M5.Lcd.print(distance);
  //M5.Lcd.println("[cm]");
  Serial.printf("%d [cm]\n", distance);
 
  if (distance <= 5 || M5.BtnB.wasPressed()) {
    crossingClose();
    crossingBlink(blinker);
    blinker = !(blinker);
  }
  if (distance > 5 || M5.BtnA.wasPressed()) {
    crossingOpen();
  }
  if (M5.BtnC.wasPressed()) {
    beepMute = !(beepMute);
  }
  delay(INTERVAL);
  M5.update();
}
