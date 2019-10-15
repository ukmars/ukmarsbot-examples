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

int decodeFunctionSwitch(int functionValue) {
  /**
   * typical ADC values for all function switch settings
   * 44, 127, 210, 267,
   * 340, 382, 423, 456,
   * 516, 541, 566, 583,
   * 609, 625, 640, 655,
   *
   * The thresholds are the mid points between these readings
   */
  const int thresholds[] = {85,  168, 238, 303, 361, 402, 439, 486,
                            528, 553, 574, 596, 617, 632, 647, 677};
  for (int i = 0; i <= 15; i++) {
    if (functionValue < thresholds[i]) {
      return i;
    }
  }
  return 16;
}

void setup() {
  Serial.begin(115200);
  Serial.println(F("Hello\n"));
  updateTime = millis() + updateInterval;
}

int i;
void loop() {
  if (millis() > updateTime) {
    updateTime += updateInterval;
    int functionValue = analogRead(FUNCTION_PIN);
    int function = decodeFunctionSwitch(functionValue);
    if (function == 16) {
      digitalWrite(LED_BUILTIN, 1);
    } else {
      digitalWrite(LED_BUILTIN, 0);
    }
    Serial.print(functionValue);
    Serial.print(F(" => "));
    Serial.println(function);
  }
}