#include <Wire.h>
#include <Adafruit_VL53L0X.h>
#include <AFMotor.h>


AF_DCMotor MotorFR(3);
AF_DCMotor MotorFL(2);
AF_DCMotor MotorBL(1);
AF_DCMotor MotorBR(4);

const int buzPin = 49;
const int ledManualPin = A8; 
const int ledAutoPin = A9;   
int valSpeed = 180;


#define NUM_SENSORS 11
Adafruit_VL53L0X sensors[NUM_SENSORS];
int xshutPins[NUM_SENSORS] = {22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32};
uint8_t sensorAddresses[NUM_SENSORS] = {
  0x30, 0x31, 0x32, 0x33, 0x34, 
  0x35, 0x36,                   
  0x37, 0x38,                   
  0x39, 0x3A                    
};

int distances[NUM_SENSORS];
bool sensorOK[NUM_SENSORS];


bool autoMode = false;


unsigned long lastBuzzerAlertMillis = 0;
const unsigned long buzzerCooldown = 2000; 


void setup() {
  Serial.begin(9600);    
  Serial1.begin(9600);  
  Wire.begin();

  pinMode(buzPin, OUTPUT);
  pinMode(ledManualPin, OUTPUT);
  pinMode(ledAutoPin, OUTPUT);

  
  autoMode = false;
  digitalWrite(ledManualPin, HIGH);
  digitalWrite(ledAutoPin, LOW);

  SetSpeed(valSpeed);
  stopAll();

  
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


void loop() {
  
  if (Serial1.available() > 0) {
    char command = Serial1.read();
    Serial1.println(command);

    if (command == 'A') {
      autoMode = true;
      digitalWrite(ledAutoPin, HIGH);
      digitalWrite(ledManualPin, LOW);
      Serial.println("Auto Mode ON");
    } else if (command == 'M') {
      autoMode = false;
      digitalWrite(ledManualPin, HIGH);
      digitalWrite(ledAutoPin, LOW);
      Serial.println("Manual Mode ON");
      stopAll();
    }

    if (!autoMode) handleManualCommand(command);
  }

  
  if (autoMode) handleAutoMode();
}


void handleManualCommand(char command) {
  switch (command) {
    case 'F': moveForward(); break;
    case 'B': moveBackward(); break;
    case 'L': moveLeft(); break;
    case 'R': moveRight(); break;
    case 'S': stopAll(); break;

    case 'Y': 
      beep(2, 150);
      break;

    case 'X': digitalWrite(ledManualPin, HIGH); break; 
    case 'x': digitalWrite(ledManualPin, LOW); break;  

    
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


void handleAutoMode() {
  int validCount = 0;

  
  for (int i = 0; i < NUM_SENSORS; i++) {
    distances[i] = readSensorAverage(i);
    if (distances[i] > 0 && distances[i] < 1000) validCount++;
  }

  
  if (validCount == 0) {
    stopAll();
    Serial.println("⚠️ Failsafe: All sensors invalid!");
    digitalWrite(ledAutoPin, HIGH); 
    beep(2, 200);
    digitalWrite(ledAutoPin, LOW); 
    delay(500);
    return;
  }

  
  bool allClose = true;
  for (int i = 0; i < NUM_SENSORS; i++) {
    if (!(distances[i] > 0 && distances[i] <= 20)) {
      allClose = false;
      break;
    }
  }

  
  if (allClose) {
    unsigned long now = millis();
    if (now - lastBuzzerAlertMillis >= buzzerCooldown) {
      rhythmicBeepPattern();
      lastBuzzerAlertMillis = now;
    }
  }

  bool frontAlert = false, leftAlert = false, rightAlert = false;

  
  for (int i = 0; i < 5; i++) if (distances[i] < 10 && distances[i] > 0) frontAlert = true;
  
  for (int i = 5; i < 7; i++) if (distances[i] < 10 && distances[i] > 0) rightAlert = true;
  
  for (int i = 7; i < 9; i++) if (distances[i] < 10 && distances[i] > 0) leftAlert = true;

  
  if (frontAlert && rightAlert && leftAlert) {
    stopAll();
  } else if (frontAlert) {
    moveRight(); 
  } else if (rightAlert) {
    moveLeft();
  } else if (leftAlert) {
    moveRight();
  } else {
    moveForward();
  }

  delay(100);
}


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


void beep(int times, int duration) {
  for (int i = 0; i < times; i++) {
    digitalWrite(buzPin, HIGH);
    delay(duration);
    digitalWrite(buzPin, LOW);
    delay(100);
  }
}


void rhythmicBeepPattern() {
  
  for (int r = 0; r < 3; r++) {
    digitalWrite(buzPin, HIGH);
    delay(100);
    digitalWrite(buzPin, LOW);
    delay(80);
  }
  delay(200); 
  for (int r = 0; r < 2; r++) {
    digitalWrite(buzPin, HIGH);
    delay(100);
    digitalWrite(buzPin, LOW);
    delay(80);
  }
}


