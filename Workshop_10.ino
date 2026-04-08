#include <Servo.h>
#include <math.h>

Servo leftServo;
Servo rightServo;

// ----------------------------
// Pin settings
// ----------------------------
const int lightPinA = A0;
const int lightPinB = A1;

const int leftServoPin  = 9;
const int rightServoPin = 10;

// ----------------------------
// Sensor values
// ----------------------------
int lightValueA = 0;
int lightValueB = 0;

// ----------------------------
// Servo output values
// ----------------------------
int leftAngle  = 90;
int rightAngle = 90;

// ----------------------------
// Sensitivity setting
// If the difference is smaller than deadband,
// treat it as balanced light.
// ----------------------------
const int deadband = 100;

void setup() {
  Serial.begin(9600);

  leftServo.attach(leftServoPin);
  rightServo.attach(rightServoPin);

  leftServo.write(90);
  rightServo.write(90);
}

void loop() {
  // Read light sensors
  lightValueA = analogRead(lightPinA);
  lightValueB = analogRead(lightPinB);

  // Compare brightness and choose behavior
  if (lightValueA > lightValueB + deadband) {
    // A is brighter
    leftAngle  = oscillate(30, 150, 200, 0);
    rightAngle = oscillate(60, 120, 200, 1);

  } else if (lightValueB > lightValueA + deadband) {
    // B is brighter
    leftAngle  = oscillate(60, 120, 1000, 0);
    rightAngle = oscillate(30, 150, 1000, 1);

  } else {
    // Balanced light
    leftAngle  = oscillate(85, 95, 3000, 0);
    rightAngle = oscillate(85, 95, 3000, 1);
  }

  // Move servos
  leftServo.write(leftAngle);
  rightServo.write(rightAngle);

  // Debug output
  Serial.print("Light A: ");
  Serial.print(lightValueA);
  Serial.print(" | Light B: ");
  Serial.print(lightValueB);
  Serial.print(" | Left: ");
  Serial.print(leftAngle);
  Serial.print(" | Right: ");
  Serial.println(rightAngle);

  delay(20);
}

// ----------------------------
// Oscillation function
// minAngle: minimum servo angle
// maxAngle: maximum servo angle
// periodMs: one full cycle time
// mode: allows different motion styles
// ----------------------------
int oscillate(int minAngle, int maxAngle, int periodMs, int mode) {
  float t = millis() % periodMs;
  float phase = t / periodMs;

  float wave;

  if (mode == 0) {
    // Smooth sine movement
    wave = sin(phase * 2.0 * PI);
  } else {
    // Cosine movement for slight variation
    wave = cos(phase * 2.0 * PI);
  }

  float normalized = (wave + 1.0) / 2.0;
  return minAngle + (maxAngle - minAngle) * normalized;
}