#include <Servo.h>
#include <AccelStepper.h>
#include <TFMPlus.h>

// Pins
const int servoPin = 6;
const int potPin = A0;
const int IN1 = 8, IN2 = 9, IN3 = 10, IN4 = 11;

// Servo Config
Servo myServo;
const int servoMinWrite = 0;    // Servo 0 = -20 degrees relative
const int servoMaxWrite = 110;  // Servo 110 = 90 degrees relative
// these are relative to the xy plane in cartesian coordinates^

// Stepper Config
AccelStepper stepper(AccelStepper::HALF4WIRE, IN1, IN3, IN2, IN4);
const int stepsPerRev = 2048;
const int thetaStepDeg = 2;
const int stepperStepSize = (stepsPerRev * thetaStepDeg) / 360;

// Lidar Config
TFMPlus tfmP;
int16_t tfDist = 0;

// Pot Calibration
int rawPotMin = 0;
int rawPotMax = 1023;

// Function to calibrate potentiometer
void calibratePotentiometer() {
  myServo.write(servoMinWrite);
  delay(600);
  rawPotMin = analogRead(potPin);

  myServo.write(servoMaxWrite);
  delay(600);
  rawPotMax = analogRead(potPin);

  Serial.print("Pot calibration -> Min: ");
  Serial.print(rawPotMin);
  Serial.print(", Max: ");
  Serial.println(rawPotMax);
}

// Function to map potentiometer reading to phi degrees
float computePhiDeg(int rawPot) {
  float norm = (rawPot - rawPotMin) / float(rawPotMax - rawPotMin);
  norm = constrain(norm, 0.0, 1.0);
  float servoAngle = norm * (servoMaxWrite - servoMinWrite) + servoMinWrite;
  float phi = map(servoAngle, 0, 110, -20, 90); // Map to real-world degrees
  return phi;
}

// Function to configure the TFMini-S for 1000Hz frame rate
void configureLidar() {
  Serial2.begin(115200);
  tfmP.begin(&Serial2);
  delay(100);

  tfmP.sendCommand(SOFT_RESET, 0);
  delay(500);
  tfmP.sendCommand(SET_SERIAL_MODE, 0);
  delay(100);
  tfmP.sendCommand(SAVE_SETTINGS, 0);
  delay(100);
  tfmP.sendCommand(SET_FRAME_RATE, 1000); // 1000Hz
  delay(100);
}

// Sweep servo down, sample pot and lidar at high rate
void sweepDownAndUp(int direction) {
  int targetAngle = (direction > 0) ? servoMaxWrite : servoMinWrite;
  myServo.write(targetAngle);
  delay(300); // Let it start moving

  unsigned long lastSampleTime = 0;
  const unsigned int sampleInterval = 2; // 500Hz sampling (~2ms)

  while (true) {
    unsigned long now = millis();
    if (now - lastSampleTime >= sampleInterval) {
      lastSampleTime = now;
      int rawPot = analogRead(potPin);
      float phiDeg = computePhiDeg(rawPot);

      if (tfmP.getData(tfDist)) {
        float r = tfDist / 100.0;
        float thetaDeg = (360.0 * stepper.currentPosition()) / stepsPerRev / 2.0; // Half scan
        Serial.print("r:");
        Serial.print(r, 2);
        Serial.print(",phi:");
        Serial.print(phiDeg);
        Serial.print(",theta:");
        Serial.println(thetaDeg);
      }
    }

    // Checking condition to see if the servo has finished moving
    int currentServo = map(analogRead(potPin), rawPotMin, rawPotMax, servoMinWrite, servoMaxWrite);
    currentServo = constrain(currentServo, servoMinWrite, servoMaxWrite);

    if (direction > 0 && currentServo >= servoMaxWrite - 2) break;
    if (direction < 0 && currentServo <= servoMinWrite + 2) break;
  }

  delay(300); // Ensure full reach
}

void setup() {
  Serial.begin(115200);
  delay(100);

  myServo.attach(servoPin);
  stepper.setMaxSpeed(1000);
  stepper.setAcceleration(500);

  configureLidar();

  calibratePotentiometer();

  stepper.setCurrentPosition(0);
}

void loop() {
  for (int currentThetaStep = 0; currentThetaStep < stepsPerRev; currentThetaStep += stepperStepSize) {
    stepper.moveTo(currentThetaStep);
    stepper.runToPosition();

    sweepDownAndUp(-1); // Sweep down
    sweepDownAndUp(1);  // Sweep up
  }

  Serial.println("Scan complete.");
  stepper.moveTo(0);
  stepper.runToPosition();

  while (true); // Halt
}
