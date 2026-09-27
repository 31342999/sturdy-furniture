#include <Wire.h>
#include <Adafruit_VL53L0X.h>
#include <AFMotor.h>

// --- Sensors & Motors ---
Adafruit_VL53L0X lox = Adafruit_VL53L0X();

AF_DCMotor motor1(1);
AF_DCMotor motor2(2);
AF_DCMotor motor3(3);
AF_DCMotor motor4(4);

int baseSpeed = 100;
int buzzerPin = 14;  // Adjust to suit your motors (0–255)

void setup() {
  Serial.begin(9600);
  pinMode(buzzerPin, OUTPUT);
  Serial.println("VL53L0X + Motor Control Starting...");

  // Initialize sensor
  if (!lox.begin()) {
    Serial.println("Failed to detect VL53L0X sensor!");
    while (1);
  }

  // Initialize motors
  motor1.setSpeed(baseSpeed);
  motor2.setSpeed(baseSpeed);
  motor3.setSpeed(baseSpeed);
  motor4.setSpeed(baseSpeed);

  motor1.run(RELEASE);
  motor2.run(RELEASE);
  motor3.run(RELEASE);
  motor4.run(RELEASE);
}

void loop() {
  VL53L0X_RangingMeasurementData_t measure;
  lox.rangingTest(&measure, false);  // Perform distance measurement

  if (measure.RangeStatus != 4) {  // If valid measurement
    int distance = measure.RangeMilliMeter / 10;  // Convert mm → cm
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");

    if (distance < 10) {
      // Object too close — stop motors
      stopAllMotors();
      Serial.println("Object detected — Motors stopped!");
      digitalWrite(buzzerPin, HIGH);
      delay(100);
      digitalWrite(buzzerPin, LOW);
      delay(100);
    } else {
      // Safe distance — move forward
      moveForward();
      Serial.println("Path clear — Moving forward");
    }
  } else {
    Serial.println("Out of range");
  }

  delay(10); // update every 100 ms
}

// --- Motor Control Functions ---
void moveForward() {
  motor1.setSpeed(baseSpeed);
  motor2.setSpeed(baseSpeed);
  motor3.setSpeed(baseSpeed);
  motor4.setSpeed(baseSpeed);

  motor1.run(FORWARD);
  motor2.run(FORWARD);
  motor3.run(FORWARD);
  motor4.run(FORWARD);
}

void stopAllMotors() {
  motor1.run(RELEASE);
  motor2.run(RELEASE);
  motor3.run(RELEASE);
  motor4.run(RELEASE);
}
