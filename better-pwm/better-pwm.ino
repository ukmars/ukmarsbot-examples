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
const float MAX_MOTOR_VOLTS = 6.0f;
const float batteryDividerRatio = 2.0f;

float gBatteryVolts;
float getBatteryVolts() {
  int adcValue = analogRead(BATTERY_VOLTS);
  gBatteryVolts = adcValue * (5.0f * batteryDividerRatio / 1023.0f);
  return gBatteryVolts;
}

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

int getFunctionSwitch() {
  int functionValue = analogRead(FUNCTION_PIN);
  int function = decodeFunctionSwitch(functionValue);
  return function;
}

void motorSetup() {
  pinMode(MOTOR_LEFT_DIR, OUTPUT);
  pinMode(MOTOR_RIGHT_DIR, OUTPUT);
  pinMode(MOTOR_LEFT_PWM, OUTPUT);
  pinMode(MOTOR_RIGHT_PWM, OUTPUT);
  digitalWrite(MOTOR_LEFT_PWM, 0);
  digitalWrite(MOTOR_LEFT_DIR, 0);
  digitalWrite(MOTOR_RIGHT_PWM, 0);
  digitalWrite(MOTOR_RIGHT_DIR, 0);
}

void setLeftMotorPWM(int pwm) {
  pwm = constrain(pwm, -255, 255);
  if (pwm < 0) {
    digitalWrite(MOTOR_LEFT_DIR, 1);
    analogWrite(MOTOR_LEFT_PWM, -pwm);
  } else {
    digitalWrite(MOTOR_LEFT_DIR, 0);
    analogWrite(MOTOR_LEFT_PWM, pwm);
  }
}

void setRightMotorPWM(int pwm) {
  pwm = constrain(pwm, -255, 255);
  if (pwm < 0) {
    digitalWrite(MOTOR_RIGHT_DIR, 0);
    analogWrite(MOTOR_RIGHT_PWM, -pwm);
  } else {
    digitalWrite(MOTOR_RIGHT_DIR, 1);
    analogWrite(MOTOR_RIGHT_PWM, pwm);
  }
}

void setMotorPWM(int left, int right) {
  setLeftMotorPWM(left);
  setRightMotorPWM(right);
}

void setLeftMotorVolts(float volts) {
  volts = constrain(volts, -MAX_MOTOR_VOLTS, MAX_MOTOR_VOLTS);
  int motorPWM = (int)((255.0f * volts) / gBatteryVolts);
  setLeftMotorPWM(motorPWM);
}

void setRightMotorVolts(float volts) {
  volts = constrain(volts, -MAX_MOTOR_VOLTS, MAX_MOTOR_VOLTS);
  int motorPWM = (int)((255.0f * volts) / gBatteryVolts);
  setRightMotorPWM(motorPWM);
}

void setMotorVolts(float left, float right) {
  setLeftMotorVolts(left);
  setRightMotorVolts(right);
}

void setup() {
  Serial.begin(115200);
  Serial.println(F("Hello\n"));
  motorSetup();
  updateTime = millis() + updateInterval;
}

void motorAction(int function) {
  switch (function) {
    case 0:
      setMotorVolts(0, 0);  // motors off
      break;
    case 1:
      setMotorVolts(1.5, 1.5);  // forward 25%
      break;
    case 2:
      setMotorVolts(3.0, 3.0);  // forward 50%
      break;
    case 3:
      setMotorVolts(4.5, 4.5);  // forward 75%
      break;
    case 4:
      setMotorVolts(-1.5, -1.5);  // reverse 25%
      break;
    case 5:
      setMotorVolts(-3.0, -3.0);  // reverse 50%
      break;
    case 6:
      setMotorVolts(-4.5, -4.5);  // reverse 75%
      break;
    case 7:
      setMotorVolts(-1.5, 1.5);  // spin left 25%
      break;
    case 8:
      setMotorVolts(-3.0, 3.0);  // spin left 50%
      break;
    case 9:
      setMotorVolts(1.5, -1.5);  // spin right 25%
      break;
    case 10:
      setMotorVolts(3.0, 3.0);  // spin right 50%
      break;
    case 11:
      setMotorVolts(0, 1.5);  // pivot left 25%
      break;
    case 12:
      setMotorVolts(1.5, 0);  // pivot right 25%
      break;
    case 13:
      setMotorVolts(1.5, 3.0);  // curve left
      break;
    case 14:
      setMotorVolts(3.0, 1.5);  // curve right
      break;
    case 15:
      setMotorVolts(4.5, 3.0);  // big curve right
      break;
    default:
      setMotorVolts(0, 0);
      break;
  }
}

void runRobot() {
  int function = getFunctionSwitch();
  // run the motors for a fixed amount of time (in milliseconds)
  uint32_t endTime = millis() + 2000;
  while (endTime > millis()) {
    getBatteryVolts();  // update the battery reading
    motorAction(function);
    if (getFunctionSwitch() == 16) {
      break;  // stop running if the button is pressed
    }
  }
  // be sure to turn off the motors
  setMotorPWM(0, 0);
  // a short delay to let everything come to rest.
  delay(500);
}

void loop() {
  if (getFunctionSwitch() == 16) {
    // button is pressed so wait until it is released
    while (getFunctionSwitch() == 16) {
      delay(20);
    }
    // now wait for the user to get clear
    delay(500);
    runRobot();
  }
}