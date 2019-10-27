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
const int SENSOR_0 = A0;
const int SENSOR_1 = A1;
const int SENSOR_2 = A2;
const int SENSOR_3 = A3;
const int SENSOR_4 = A4;
const int SENSOR_5 = A5;
const int FUNCTION_PIN = A6;
const int BATTERY_VOLTS = A7;
/****/

/***
 * Global variables
 */

uint32_t updateTime;
uint32_t updateInterval = 200;  // in milliseconds

const int SENSOR_CHANNELS = 6;

const float LINE_WIDTH = 19.0;  // ADJUST THIS so that CTE is roughly equal to the error in mm
const float LINE_DETECT_THRESHOLD = 900.0;  // minimum value to register the line - ADJUST TO SUIT
const float LEFT_MARKER_THRESHOLD = 180.0;  // minimum value to register the turn marker
const float RIGHT_MARKER_THRESHOLD = 180.0; // minimum value to register the start marker
volatile float gSensorStart;
volatile float gSensorTurn;
volatile float gSensorRight;
volatile float gSensorLeft;

volatile float gSensorSum;
volatile float gSensorDifference;
volatile float gSensorCTE;


void analogueSetup() {
  // increase speed of ADC conversions to 28us each
  // by changing the clock prescaler from 128 to 16
  bitClear(ADCSRA, ADPS0);
  bitClear(ADCSRA, ADPS1);
  bitSet(ADCSRA, ADPS2);
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

void updateLineSensor() {
  // read ALL the sensor channels;
  int sensorDark[SENSOR_CHANNELS] = {0};
  int sensorLit[SENSOR_CHANNELS] = {0};
  int sensorValue[SENSOR_CHANNELS] = {0};
  for (int i = 0; i < SENSOR_CHANNELS; i++) {
    sensorDark[i] = analogRead(A0 + i);
  }
  digitalWrite(EMITTER, 1);
  delayMicroseconds(50);
  for (int i = 0; i < SENSOR_CHANNELS; i++) {
    sensorLit[i] = analogRead(A0 + i);
  }
  digitalWrite(EMITTER, 0);
  for (int i = 0; i < SENSOR_CHANNELS; i++) {
    sensorValue[i] = abs(sensorLit[i] - sensorDark[i]);
  }
  gSensorStart = sensorValue[0];
  gSensorRight = sensorValue[1];
  gSensorLeft = sensorValue[2];
  gSensorTurn = sensorValue[3];
  if (gSensorStart > RIGHT_MARKER_THRESHOLD) {
    digitalWrite(LED_RIGHT, 1);
  } else {
    digitalWrite(LED_RIGHT, 0);
  }
  if (gSensorTurn > LEFT_MARKER_THRESHOLD) {
    digitalWrite(LED_LEFT, 1);
  } else {
    digitalWrite(LED_LEFT, 0);
  }
  gSensorSum = gSensorRight + gSensorLeft;
  gSensorDifference = gSensorRight - gSensorLeft;
  if (gSensorSum > LINE_DETECT_THRESHOLD) {
    gSensorCTE = LINE_WIDTH * (gSensorDifference / gSensorSum);
  } else {
    gSensorCTE = 0;
  }
}
// the systick event is an ISR attached to Timer 2
ISR(TIMER2_COMPA_vect) {
  updateLineSensor();
}

void setup() {
  Serial.begin(9600);
  pinMode(EMITTER, OUTPUT);
  pinMode(LED_RIGHT, OUTPUT);
  pinMode(LED_LEFT, OUTPUT);
  digitalWrite(EMITTER, 0);  // be sure the emitter is off
  analogueSetup();           // increase the ADC conversion speed
  setupSystick();
  updateTime = millis() + updateInterval;
}

void loop() {
  if (millis() > updateTime) {
    updateTime += updateInterval;
    Serial.print(F("  Start: "));
    Serial.print(gSensorStart);
    Serial.print(F("  Turn: "));
    Serial.print(gSensorTurn);
    Serial.print(F("  Right: "));
    Serial.print(gSensorRight);
    Serial.print(F("  Left: "));
    Serial.print(gSensorLeft);
    Serial.print(F("  Sum: "));
    Serial.print(gSensorSum);
    Serial.print(F("  Diff: "));
    Serial.print(gSensorDifference);
    Serial.print(F("  CTE: "));
    Serial.print(gSensorCTE);
    Serial.println();
  }
}