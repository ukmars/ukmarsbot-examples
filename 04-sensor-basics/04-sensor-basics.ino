#include <Arduino.h>
/**
 * Hardware pin defines
 */
#define BOARD UKMARSBOT_V1
const int ENCODER_LEFT_CLK = 2;
const int ENCODER_RIGHT_CLK = 3;
const int ENCODER_LEFT_B = 4;
const int ENCODER_RIGHT_B = 5;
const int MOTOR_LEFT_DIR = 7;
const int MOTOR_RIGHT_DIR = 8;
const int MOTOR_LEFT_PWM = 9;
const int MOTOR_RIGHT_PWM = 10;
const int LED_RIGHT = 6;
const int LED_LEFT = 11;
const int EMITTER = 12;
const int SENSOR_RIGHT_MARK = A0;
const int SENSOR_1 = A1;
const int SENSOR_2 = A2;
const int SENSOR_3 = A3;
const int SENSOR_4 = A4;
const int SENSOR_LEFT_MARK = A5;
const int FUNCTION_PIN = A6;
const int BATTERY_VOLTS = A7;
/****/

/***
 * Global variables
 */

uint32_t updateTime;
uint32_t updateInterval = 100;  // in milliseconds

int sensorDark = 0;
int sensorLit = 0;
int sensorValue = 0;

int readSensor(int channel){
  sensorDark = analogRead(channel);
  digitalWrite(EMITTER,1);
  delayMicroseconds(50);
  sensorLit = analogRead(channel);
  digitalWrite(EMITTER,0);
  sensorValue = sensorLit-sensorDark;
  return sensorValue;
}

void setup() {
  Serial.begin(115200);
  Serial.println(F("Sensor Basics\n"));
  pinMode(EMITTER,OUTPUT);
  digitalWrite(EMITTER,0); // be sure the emitter is off
  updateTime = millis() + updateInterval;
}


void loop() {
  if (millis() > updateTime) {
    updateTime += updateInterval;
    readSensor(A0);
    Serial.print(sensorDark);
    Serial.print(F(" > "));
    Serial.print(sensorLit);
    Serial.print(F(" = "));
    Serial.print(sensorValue);
    Serial.println();
  }
}
