#include <Arduino.h>
#include <Wire.h>
#include <stdlib.h>

// STM32 NUCLEO-C031C6 prototype pin assignments
const int RPM_PIN = A0;
const int TEMP_PIN = A1;
const int CURRENT_PIN = A2;
const int BUZZER_PIN = D6;

const uint8_t MPU_ADDR = 0x68;
const uint8_t MPU_PWR_MGMT_1 = 0x6B;
const uint8_t MPU_ACCEL_XOUT_H = 0x3B;

int16_t previousAx = 0;
int16_t previousAy = 0;
int16_t previousAz = 0;
bool havePreviousSample = false;

bool writeMPURegister(uint8_t reg, uint8_t value) {
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(reg);
  Wire.write(value);
  return Wire.endTransmission() == 0;
}

bool readMPUAcceleration(int16_t &ax, int16_t &ay, int16_t &az) {
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(MPU_ACCEL_XOUT_H);

  // Use a normal endTransmission() followed by requestFrom().
  // This avoids repeated-start compatibility issues seen with some core versions.
  if (Wire.endTransmission() != 0) {
    return false;
  }

  uint8_t received = Wire.requestFrom(MPU_ADDR, (uint8_t)6);
  if (received != 6) {
    while (Wire.available()) Wire.read();
    return false;
  }

  ax = (int16_t)((Wire.read() << 8) | Wire.read());
  ay = (int16_t)((Wire.read() << 8) | Wire.read());
  az = (int16_t)((Wire.read() << 8) | Wire.read());
  return true;
}

bool mpuReady = false;
int vibrationLevel = 0;

void setup() {
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);

  Serial.begin(115200);
  delay(1000);

  analogReadResolution(12);
  Wire.begin();
  Wire.setClock(100000);

  Serial.println("STM32 STARTED");
  Serial.println("MARINE UAV MONITOR");

  mpuReady = writeMPURegister(MPU_PWR_MGMT_1, 0x00);
  delay(100);

  Serial.println("SYSTEM INITIALIZED");
}

void loop() {
  const int rawRPM = analogRead(RPM_PIN);
  const int rawTemp = analogRead(TEMP_PIN);
  const int rawCurrent = analogRead(CURRENT_PIN);

  // Demonstration scaling only; replace with calibrated sensor conversions.
  const int rpm = (long)rawRPM * 5000 / 4095;
  const int temperature = (long)rawTemp * 120 / 4095;
  const int currentValue = (long)rawCurrent * 30 / 4095;

  int16_t ax = 0, ay = 0, az = 0;
  mpuReady = readMPUAcceleration(ax, ay, az);

  if (mpuReady) {
    if (havePreviousSample) {
      vibrationLevel = abs((int)ax - previousAx)
                     + abs((int)ay - previousAy)
                     + abs((int)az - previousAz);
    } else {
      vibrationLevel = 0;
      havePreviousSample = true;
    }
    previousAx = ax;
    previousAy = ay;
    previousAz = az;
  } else {
    vibrationLevel = 0;
  }

  const char *status = "NORMAL";
  bool critical = false;

  if (!mpuReady || temperature >= 100 || currentValue >= 25 ||
      vibrationLevel >= 3000) {
    status = "CRITICAL";
    critical = true;
  } else if (temperature >= 80 || currentValue >= 20 ||
             vibrationLevel >= 1500 || rpm < 1000) {
    status = "WARNING";
  }

  digitalWrite(BUZZER_PIN, critical ? HIGH : LOW);

  Serial.println();
  Serial.println("MARINE UAV HEALTH MONITOR");
  Serial.print("RPM          : "); Serial.println(rpm);
  Serial.print("TEMPERATURE  : "); Serial.print(temperature); Serial.println(" C");
  Serial.print("CURRENT      : "); Serial.print(currentValue); Serial.println(" A");
  Serial.print("VIBRATION    : "); Serial.println(vibrationLevel);
  Serial.print("MPU6050      : "); Serial.println(mpuReady ? "READY" : "NOT READY");
  Serial.print("SYSTEM STATUS: "); Serial.println(status);
  Serial.println("----------------------------");

  delay(1000);
}
