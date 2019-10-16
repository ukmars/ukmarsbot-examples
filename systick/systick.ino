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

float gBatteryVolts;
int gFunctionSwitch;

uint32_t gUpdateTime;
const uint32_t gUpdateInterval = 100;  // in milliseconds
const float batteryDividerRatio = 2.0f;

/***/

void updateBatteryVolts() {
  int adcValue = analogRead(BATTERY_VOLTS);
  gBatteryVolts = adcValue * (5.0f * batteryDividerRatio / 1023.0f);
}

void updateFunctionSwitch() {
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
  int adcValue = analogRead(FUNCTION_PIN);
  for (int i = 0; i <= 15; i++) {
    if (adcValue < thresholds[i]) {
      gFunctionSwitch = i;
      return;
    }
  }
  gFunctionSwitch = 16;
}

/***
 * If you are interested in what all this does, the ATMega328P datasheet
 * has all the answers but it is not easy to follow until you have some
 * experience. For now just use the code as it is.
 */
void setupSystick() {
  // set the mode for timer 2
  bitClear(TCCR2A, WGM20);
  bitSet(TCCR2A, WGM21);
  bitClear(TCCR2B, WGM22);
  // set divisor to 128 => timer clock = 125kHz
  bitSet(TCCR2B, CS22);
  bitClear(TCCR2B, CS21);
  bitSet(TCCR2B, CS20);
  // set the timer frequency
  OCR2A = 249;  // (16000000/128/500)-1 = 249
  // enable the timer interrupt
  bitSet(TIMSK2, OCIE2A);
}

// the systick event is an ISR attached to Timer 2
ISR(TIMER2_COMPA_vect) {
  updateBatteryVolts();
  updateFunctionSwitch();
}

void setup() {
  Serial.begin(115200);
  Serial.println(F("Systick Demo\n"));
  setupSystick();
  gUpdateTime = millis() + gUpdateInterval;
}

void loop() {
  if (millis() > gUpdateTime) {
    gUpdateTime += gUpdateInterval;
    if (gFunctionSwitch == 16) {
      digitalWrite(LED_BUILTIN, 1);
    } else {
      digitalWrite(LED_BUILTIN, 0);
    }
    Serial.print(F("Battery = "));
    Serial.print(gBatteryVolts);
    Serial.print(F("    Function = "));
    Serial.print(gFunctionSwitch);
    Serial.println();
  }
}