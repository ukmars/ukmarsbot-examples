#include <Arduino.h>
#include "digitalWriteFast.h"
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

const float batteryDividerRatio = 2.0f;

float getBatteryVolts(int adcValue) {
  float batteryVolts = adcValue * (5.0f * batteryDividerRatio / 1023.0f);
  return batteryVolts;
}

void setup() {
  Serial.begin(115200);
  Serial.println(F("Sensor Basics\n"));
  updateTime = millis() + updateInterval;
}

void loop() {
  if (millis() > updateTime) {
    updateTime += updateInterval;
    int adcValue = analogRead(BATTERY_VOLTS);
    Serial.print(adcValue);
    Serial.print(F(" => "));
    Serial.print(getBatteryVolts(adcValue));
    Serial.print(F(" Volts "));
    Serial.println();
  }
}