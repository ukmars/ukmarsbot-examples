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
 * Global robot characteristic constants
 */
const int COUNTS_PER_ROTATION = 12;
const float GEAR_RATIO = 19.5;
const float WHEEL_DIAMETER = 32.5f;
const float WHEEL_SEPARATION = 75.2;

const float MM_PER_COUNT = (PI * WHEEL_DIAMETER) / (2 * COUNTS_PER_ROTATION * GEAR_RATIO);
const float DEG_PER_COUNT = (360.0 * MM_PER_COUNT) / (PI * WHEEL_SEPARATION);
/***
 * Global variables
 */
volatile int32_t encoderLeftCount;
volatile int32_t encoderRightCount;
int32_t encoderSum;
int32_t encoderDifference;
uint32_t updateTime;
uint32_t updateInterval = 100;  // in milliseconds

void setupEncoder() {
  // left
  pinMode(ENCODER_LEFT_CLK, INPUT);
  pinMode(ENCODER_LEFT_B, INPUT);
  // configure the pin change
  bitClear(EICRA, ISC01);
  bitSet(EICRA, ISC00);
  // enable the interrupt
  bitSet(EIMSK, INT0);
  encoderLeftCount = 0;
  // right
  pinMode(ENCODER_RIGHT_CLK, INPUT);
  pinMode(ENCODER_RIGHT_B, INPUT);
  // configure the pin change
  bitClear(EICRA, ISC11);
  bitSet(EICRA, ISC10);
  // enable the interrupt
  bitSet(EIMSK, INT1);
  encoderRightCount = 0;
}

ISR(INT0_vect) {
  static bool oldB = 0;
  bool newB = bool(digitalReadFast(ENCODER_LEFT_B));
  bool newA = bool(digitalReadFast(ENCODER_LEFT_CLK)) ^ newB;
  if (newA == oldB) {
    encoderLeftCount--;
  } else {
    encoderLeftCount++;
  }
  oldB = newB;
}

ISR(INT1_vect) {
  static bool oldB = 0;
  bool newB = bool(digitalReadFast(ENCODER_RIGHT_B));
  bool newA = bool(digitalReadFast(ENCODER_RIGHT_CLK)) ^ newB;
  if (newA == oldB) {
    encoderRightCount++;
  } else {
    encoderRightCount--;
  }
  oldB = newB;
}

int decodeFunctionSwitch(int functionValue) {
  /**
   * Typical ADC values for all function switch settings
   */
  const int adcReading[] = {660, 647, 630, 614, 590, 570, 545, 522, 461,
                            429, 385, 343, 271, 212, 128, 44,  0};

  if (functionValue > 1000) {
    return 16;  // pushbutton closed
  }
  int result = 16;
  for (int i = 0; i < 16; i++) {
    if (functionValue > (adcReading[i] + adcReading[i + 1]) / 2) {
      result = i;
      break;
    }
  }
  return result;
}

int getFunctionSwitch() {
  int functionValue = analogRead(FUNCTION_PIN);
  int function = decodeFunctionSwitch(functionValue);
  return function;
}

void setup() {
  Serial.begin(9600);
  setupEncoder();
  Serial.println(F("Hello\n"));
  updateTime = millis() + updateInterval;
  Serial.print(F("MM PER COUNT = "));
  Serial.println(MM_PER_COUNT, 5);

  Serial.print(F("So 500mm travel would be 500/"));
  Serial.print(MM_PER_COUNT, 5);
  Serial.print(F(" = "));
  Serial.print(500.0/MM_PER_COUNT);
  Serial.println(F(" counts"));
  Serial.println();

  Serial.print(F("DEG PER COUNT = "));
  Serial.println(DEG_PER_COUNT, 5);

  Serial.print(F("So 360 degrees rotation would be 360/"));
  Serial.print(DEG_PER_COUNT, 5);
  Serial.print(F(" = "));
  Serial.print(360.0/DEG_PER_COUNT);
  Serial.println(F(" counts"));
  Serial.println();
  Serial.println(F("Press the user button to continue..."));
  while(getFunctionSwitch() != 16){
    // do nothing until the pushbutton is pressed
  }
  Serial.println();
}

void loop() {
  if (millis() > updateTime) {
    updateTime += updateInterval;
    if(getFunctionSwitch() == 16){
        encoderLeftCount = 0;
        encoderRightCount = 0;
    }
    encoderSum = encoderRightCount + encoderLeftCount;
    encoderDifference = encoderRightCount - encoderLeftCount;
    float distance = MM_PER_COUNT * encoderSum;
    float angle = DEG_PER_COUNT * encoderDifference;

    Serial.print(F("EncoderSum: "));
    Serial.print(encoderSum);
    Serial.print(F(" = "));
    Serial.print(distance);
    Serial.print(F(" mm    "));

    Serial.print(F("EncoderDifference: "));
    Serial.print(encoderDifference);
    Serial.print(F(" = "));
    Serial.print(angle);
    Serial.print(F(" deg"));


    Serial.println();
  }
}