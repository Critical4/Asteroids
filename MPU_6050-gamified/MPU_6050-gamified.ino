#include <Wire.h>

const int MPU_ADDR = 0x68; // Default I2C address for MPU6050
int16_t ax, ay, az;

// Adjust this threshold if the tilt feels too sensitive or too stiff (range: ~3000 to 8000)
const int TILT_THRESHOLD = 4500;

void setup() {
  Serial.begin(9600); // Matches baudRate: 9600 in your HTML
  Wire.begin();
  
  // Wake up the MPU6050
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x6B); // PWR_MGMT_1 register
  Wire.write(0);    // Set to 0 to wake up the sensor
  Wire.endTransmission(true);
}

void loop() {
  // Request accelerometer data (6 bytes)
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B); // Starting register for Accel X
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDR, 6, true);

  ax = Wire.read() << 8 | Wire.read(); // X-axis tilt
  ay = Wire.read() << 8 | Wire.read(); // Y-axis tilt
  az = Wire.read() << 8 | Wire.read(); // Z-axis

  // Determine direction and print exact text expected by your game
  if (ax < -TILT_THRESHOLD) {
    Serial.println("left");
  } else if (ax > TILT_THRESHOLD) {
    Serial.println("right");
  } else if (ay < -TILT_THRESHOLD) {
    Serial.println("up");
  } else if (ay > TILT_THRESHOLD) {
    Serial.println("down");
  } else {
    Serial.println("flat");
  }

  delay(50); // Sends updates 20 times per second (prevents "No Signal" timeout)
}