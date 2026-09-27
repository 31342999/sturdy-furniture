#include <Wire.h>
#include <Adafruit_VL53L0X.h>

Adafruit_VL53L0X lox = Adafruit_VL53L0X();

const int buzzerPin = 8;     // Buzzer pin
const int threshold_cm = 20; // Distance threshold in cm

void setup() {
  Serial.begin(9600);
  pinMode(buzzerPin, OUTPUT);

  Serial.println("Starting VL53L0X...");
  if (!lox.begin()) {
    Serial.println("Failed to boot VL53L0X");
    while (1);
  }
}

void loop() {
  VL53L0X_RangingMeasurementData_t measure;
  
  lox.rangingTest(&measure, false); // Pass in 'true' to get debug data

  if (measure.RangeStatus == 4) {
    // Sensor out of range
    Serial.println("Out of range");
    digitalWrite(buzzerPin, LOW);
  } else {
    int distance = measure.RangeMilliMeter / 10; // convert mm → cm
    Serial.print("Distance (cm): ");
    Serial.println(distance);

    if (distance < threshold_cm) {
      digitalWrite(buzzerPin, HIGH); // Trigger buzzer
    } else {
      digitalWrite(buzzerPin, LOW);  // Turn buzzer off
    }
  }

  delay(100);
}
