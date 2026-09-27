#include <Wire.h>
#include <Adafruit_VL53L0X.h>
#include <AFMotor.h>

// ---------------- MOTOR SETUP ----------------
AF_DCMotor MotorFR(4);
AF_DCMotor MotorFL(1);
AF_DCMotor MotorBL(2);
AF_DCMotor MotorBR(3);

const int buzPin = 2;
const int ledPin = A5;
int valSpeed = 180;

// ---------------- SENSOR SETUP ----------------
#define NUM_SENSORS 11
Adafruit_VL53L0X sensors[NUM_SENSORS];
int xshutPins[NUM_SENSORS] = {22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32};
uint8_t sensorAddresses[NUM_SENSORS] = {
  0x30, 0x31, 0x32, 0x33, 0x34, // Front (5)
  0x35, 0x36,                   // Right (2)
  0x37, 0x38,                   // Left (2)
  0x39, 0x3A                    // Back (2)
};

int distances[NUM_SENSORS];
bool sensorOK[NUM_SENSORS];

// Mode flag
bool autoMode = false;

// ---------------- SETUP ----------------
void setup() {
  Serial.begin(9600);    // USB debug
  Serial1.begin(9600);   // Bluetooth
  Wire.begin();

  pinMode(buzPin, OUTPUT);
  pinMode(ledPin, OUTPUT);

  SetSpeed(valSpeed);
  stopAll();

  // Initialize all sensors
  for (int i = 0; i < NUM_SENSORS; i++) {
    pinMode(xshutPins[i], OUTPUT);
    digitalWrite(xshutPins[i], LOW);
  }
  delay(100);

  for (int i = 0; i < NUM_SENSORS; i++) {
    digitalWrite(xshutPins[i], HIGH);
    delay(10);
    if (!sensors[i].begin(sensorAddresses[i], false, &Wire)) {
      Serial.print("Sensor "); Serial.print(i); Serial.println(" failed!");
      sensorOK[i] = false;
    } else {
      Serial.print("Sensor "); Serial.print(i); Serial.println(" ready.");
      sensorOK[i] = true;
    }
  }

  Serial.println("Robot initialized! Waiting for Bluetooth...");
}

// ---------------- MAIN LOOP ----------------
void loop() {
  // Handle Bluetooth input
  if (Serial1.available() > 0) {
    char command = Serial1.read();
    Serial.print("BT Command: ");
    Serial.println(command);
    Serial1.print("Received: ");
    Serial1.println(command);

    if (command == 'A') {
      autoMode = true;
      Serial.println("Auto Mode ON");
    } else if (command == 'M') {
      autoMode = false;
      Serial.println("Manual Mode ON");
      stopAll();
    }

    if (!autoMode) handleManualCommand(command);
  }

  // Run autonomous mode
  if (autoMode) handleAutoMode();
}

// ---------------- MANUAL MODE HANDLER ----------------
void handleManualCommand(char command) {
  switch (command) {
    case 'F': moveForward(); break;
    case 'B': moveBackward(); break;
    case 'L': moveLeft(); break;
    case 'R': moveRight(); break;
    case 'S': stopAll(); break;

    case 'Y': // Buzzer honk
      beep(2, 150);
      break;

    case 'X': digitalWrite(ledPin, HIGH); break; // LED on
    case 'x': digitalWrite(ledPin, LOW); break;  // LED off

    // Speed control
    case '0': SetSpeed(0); break;
    case '1': SetSpeed(50); break;
    case '2': SetSpeed(75); break;
    case '3': SetSpeed(100); break;
    case '4': SetSpeed(125); break;
    case '5': SetSpeed(150); break;
    case '6': SetSpeed(175); break;
    case '7': SetSpeed(200); break;
    case '8': SetSpeed(225); break;
    case '9': SetSpeed(255); break;
  }
}

// ---------------- AUTO MODE HANDLER ----------------
void handleAutoMode() {
  int validCount = 0;

  // Read all sensors with averaging
  for (int i = 0; i < NUM_SENSORS; i++) {
    distances[i] = readSensorAverage(i);
    if (distances[i] > 0 && distances[i] < 1000) validCount++;
  }

  // FAILSAFE: If no valid readings
  if (validCount == 0) {
    stopAll();
    Serial.println("⚠️ Failsafe: All sensors invalid!");
    digitalWrite(ledPin, HIGH);
    beep(2, 200);
    digitalWrite(ledPin, LOW);
    delay(500);
    return;
  }

  bool frontAlert = false, leftAlert = false, rightAlert = false;

  // Front sensors 0–4
  for (int i = 0; i < 5; i++) if (distances[i] < 10 && distances[i] > 0) frontAlert = true;
  // Right sensors 5–6
  for (int i = 5; i < 7; i++) if (distances[i] < 10 && distances[i] > 0) rightAlert = true;
  // Left sensors 7–8
  for (int i = 7; i < 9; i++) if (distances[i] < 10 && distances[i] > 0) leftAlert = true;

  // Decision logic
  if (frontAlert && rightAlert && leftAlert) {
    stopAll();
  } else if (frontAlert) {
    moveRight(); // priority right
  } else if (rightAlert) {
    moveLeft();
  } else if (leftAlert) {
    moveRight();
  } else {
    moveForward();
  }

  delay(100);
}

// ---------------- SENSOR AVERAGING FUNCTION ----------------
int readSensorAverage(int index) {
  if (!sensorOK[index]) return -1;

  int validReadings = 0;
  long total = 0;

  for (int i = 0; i < 3; i++) {
    VL53L0X_RangingMeasurementData_t measure;
    sensors[index].rangingTest(&measure, false);
    if (measure.RangeStatus != 4) {
      total += measure.RangeMilliMeter / 10;
      validReadings++;
    }
    delay(10);
  }

  if (validReadings == 0) {
    sensorOK[index] = false;
    return -1;
  } else {
    sensorOK[index] = true;
    return total / validReadings;
  }
}

// ---------------- MOTOR FUNCTIONS ----------------
void moveForward() {
  SetSpeed(valSpeed);
  MotorFL.run(FORWARD);
  MotorFR.run(FORWARD);
  MotorBL.run(FORWARD);
  MotorBR.run(FORWARD);
}

void moveBackward() {
  SetSpeed(valSpeed);
  MotorFL.run(BACKWARD);
  MotorFR.run(BACKWARD);
  MotorBL.run(BACKWARD);
  MotorBR.run(BACKWARD);
}

void moveLeft() {
  SetSpeed(valSpeed);
  MotorFL.run(BACKWARD);
  MotorFR.run(FORWARD);
  MotorBL.run(BACKWARD);
  MotorBR.run(FORWARD);
}

void moveRight() {
  SetSpeed(valSpeed);
  MotorFL.run(FORWARD);
  MotorFR.run(BACKWARD);
  MotorBL.run(FORWARD);
  MotorBR.run(BACKWARD);
}

void stopAll() {
  MotorFL.run(RELEASE);
  MotorFR.run(RELEASE);
  MotorBL.run(RELEASE);
  MotorBR.run(RELEASE);
}

void SetSpeed(int val) {
  valSpeed = val;
  MotorFL.setSpeed(val);
  MotorFR.setSpeed(val);
  MotorBL.setSpeed(val);
  MotorBR.setSpeed(val);
}

// ---------------- BUZZER FUNCTION ----------------
void beep(int times, int duration) {
  for (int i = 0; i < times; i++) {
    digitalWrite(buzPin, HIGH);
    delay(duration);
    digitalWrite(buzPin, LOW);
    delay(100);
  }
}
