//
// Crossing
// 2018-08-21 rev 0.1 initial
// 2018-08-30 rev 0.2 mp3 working
// 2018-09-01 rev 0.3 LED blinker
//
#include <M5Stack.h>
//#include <WiFi.h>
#include <Ultrasonic.h>
/*
#include "AudioFileSourceSD.h"
#include "AudioFileSourceID3.h"
#include "AudioGeneratorMP3.h"
#include "AudioOutputI2S.h"
*/

#define ULTRASONIC_PIN  (22)
#define INTERVAL        (300)
#define SERVO_PIN       (5)
#define LED1_PIN        (16)
#define LED2_PIN        (17)

Ultrasonic UltrasonicRanger(ULTRASONIC_PIN);
bool closed = false;
bool blinker = false;

int currentAngle = 0;
// Speaker
/*
#define NOTE_D2 330
#define NOTE_D3F 340
#define NOTE_D3 350
*/
// MP3
/*
AudioGeneratorMP3 *mp3;
AudioFileSourceSD *file;
AudioOutputI2S *out;
AudioFileSourceID3 *id3;
*/
/*
void setupMP3() 
{
  //mp3->begin(id3, out);
}
void startMP3()
{
  if (!mp3 || !mp3->isRunning()) {
    M5.Lcd.print("play MP3");
    file = new AudioFileSourceSD("/pno-cs.mp3");
    id3 = new AudioFileSourceID3(file);
    out = new AudioOutputI2S(0, 1); // Output to builtInDAC
    out->SetOutputModeMono(true);
    mp3 = new AudioGeneratorMP3();
    mp3->begin(id3, out);    
  }
}
void stopMP3()
{
  if (mp3) {
    M5.Lcd.print("stop MP3 ");
    mp3->stop();
    //delete mp3;
    //mp3 = NULL;
  }
  if (out) {
    //out->close();
    delete out;
    out = NULL;
  }
  if (id3) {
    id3->close();
    delete id3;
    id3 = NULL;
  }
  if (file) {
    file->close();
    delete file;
    file = NULL;
  }
}
*/
/*
int servoPulse(int angleDegrees)
{
  int pulseWidth = map(angleDegrees, 0, 180, 544, 2400);
  return pulseWidth;
}

int servoSet(int angle)
{
   int pulseWidth;
   pulseWidth = servoPulse(angle);
   digitalWrite(SERVO_PIN, HIGH);
   delayMicroseconds(pulseWidth);
   digitalWrite(SERVO_PIN, LOW);
   delayMicroseconds(20000 - pulseWidth);
   return 0;
}
*/
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
  //M5.Lcd.println("Close crossing");
  //M5.Lcd.fillScreen(BLACK);
  M5.Lcd.fillCircle(100, 100, 50, RED);
  M5.Lcd.fillCircle(240, 100, 50, RED);
  //M5.Lcd.fillScreen(RED);
  servoSet(currentAngle, 90);
  currentAngle = 90;
}
void crossingOpen()
{
  servoSet(currentAngle, 0);
  currentAngle = 0;
  //M5.Lcd.fillScreen(BLACK);
  M5.Lcd.fillCircle(100, 100, 50, BLACK);
  M5.Lcd.fillCircle(240, 100, 50, BLACK);
  //digitalWrite(LED1_PIN, LOW);
  //digitalWrite(LED2_PIN, LOW);
}

void crossingBlink(bool blinker) {
  //M5.Speaker.setVolume(0);
  if (blinker) {
    M5.Lcd.fillCircle(100, 100, 50, RED);
    M5.Lcd.fillCircle(240, 100, 50, BLACK);
    //M5.Speaker.tone(442, 10);
    //digitalWrite(LED1_PIN, HIGH);
    //digitalWrite(LED2_PIN, LOW);
  }
  else {
    M5.Lcd.fillCircle(100, 100, 50, BLACK);
    M5.Lcd.fillCircle(240, 100, 50, RED);
    //M5.Speaker.tone(440, 10);
    //digitalWrite(LED1_PIN, LOW);
    //digitalWrite(LED2_PIN, HIGH);
  }
}

void setup(){
  M5.begin();
  //WiFi.mode(WIFI_OFF); 
  delay(500);
  //setupMP3();
  //startMP3();
  
  pinMode(SERVO_PIN, OUTPUT);
  //pinMode(LED1_PIN, OUTPUT);
  //pinMode(LED2_PIN, OUTPUT);
  //digitalWrite(LED1_PIN, LOW);
  //digitalWrite(LED2_PIN, LOW);
  M5.Lcd.fillCircle(100, 100, 60, TFT_DARKGREY);
  M5.Lcd.fillCircle(240, 100, 60, TFT_DARKGREY);
  delay(1000);
  //stopMP3();
}


void loop(){
  long distance;
  distance = UltrasonicRanger.MeasureInCentimeters();
  //distance = 1000;
  /*
  M5.Lcd.print(closed);
  M5.Lcd.print(", ");
  M5.Lcd.print(distance);
  M5.Lcd.println("[cm]");
  */
  if (distance <= 5 || M5.BtnB.wasPressed()) {
    crossingClose();
    closed = true;
  }
  if (distance > 5 || M5.BtnA.wasPressed()) {
    crossingOpen();
    closed = false;
  }
  if (closed) {
    crossingBlink(blinker);
    blinker = !(blinker);
  }
  delay(INTERVAL);
  M5.update();

}


void loop_old1(){
  long distance;
  //distance = UltrasonicRanger.MeasureInCentimeters();
  //distance = 1000;
  /*
  M5.Lcd.print(closed);
  M5.Lcd.print(", ");
  M5.Lcd.print(distance);
  M5.Lcd.println("[cm]");
  */
  if (closed) {
    crossingBlink(blinker);
    blinker = !(blinker);
    // Open crossing
    if (distance > 5 || M5.BtnB.wasPressed()) {
      //M5.Speaker.mute();
      // Delay prior to open
      delay(1500);
      //stopMP3();
      //crossingOpen();
      closed = false;
    }
  }
  else {
    // Close crossing
    if (distance < 5 || M5.BtnC.wasPressed()) {
      //startMP3();
      //crossingClose();
      closed = true;
    }
  }
  delay(INTERVAL);
  M5.update();

}
