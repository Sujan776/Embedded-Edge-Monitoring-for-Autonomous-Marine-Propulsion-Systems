# Real-Time Propulsion Health Monitoring and Fault-Tolerant Control for an Autonomous Marine UAV

## Project overview
An STM32-based proof-of-concept for monitoring propulsion-related parameters in an autonomous marine UAV.

The prototype reads:
- Simulated RPM from a potentiometer
- Simulated temperature from a potentiometer
- Simulated current from a potentiometer
- Motion-related acceleration changes from an MPU6050

The STM32 classifies the operating condition as **NORMAL**, **WARNING**, or **CRITICAL** and activates a buzzer for a critical condition.

> **Prototype limitation:** RPM, temperature and current are currently simulated using potentiometers. The MPU6050 provides raw acceleration-change-based motion indication; it is not yet a calibrated vibration measurement. The project has not yet been validated on a real marine propulsion motor or vessel.

## Hardware
- STM32 NUCLEO-C031C6
- MPU6050 module (I2C address `0x68`)
- 3 potentiometers
- Active buzzer (verify module polarity/type)
- Jumper wires

## Wiring

| Component | STM32 connection |
|---|---|
| RPM potentiometer wiper | A0 |
| Temperature potentiometer wiper | A1 |
| Current potentiometer wiper | A2 |
| MPU6050 SDA | PB9 |
| MPU6050 SCL | PB8 |
| MPU6050 VCC | 3.3V |
| MPU6050 GND | GND |
| Buzzer positive / signal | D6 |
| Buzzer negative / GND | GND |
| Potentiometer outer pins | 3.3V and GND |

All modules must share a common GND. Keep analog inputs within the STM32 board's permitted input voltage (use 3.3V, not 5V).

## Software
The source is in `src/main.ino`. It is written for the Arduino STM32 core. Select the NUCLEO-C031C6 board in your Arduino-compatible STM32 development environment, select the correct serial port, and upload.

Open Serial Monitor at **115200 baud**.

## Demonstration
1. Turn the A0 potentiometer and observe RPM.
2. Turn A1 and observe temperature.
3. Turn A2 and observe current.
4. Gently move the MPU6050 module and observe the motion/vibration indicator.
5. Increase the simulated temperature to the critical threshold (100 °C) and verify the buzzer.

## Demonstration thresholds in this prototype
- CRITICAL: MPU6050 unavailable, temperature >= 100 °C, current >= 25 A, or vibration indicator >= 3000
- WARNING: temperature >= 80 °C, current >= 20 A, vibration indicator >= 1500, or RPM < 1000
- Otherwise: NORMAL

These are demonstration thresholds only, not validated marine safety limits.

## Example serial output
See `docs/example_serial_output.txt`. Your actual values will vary with potentiometer positions and sensor movement.

## Repository contents
- `src/main.ino` — firmware
- `hardware/wiring.md` — wiring reference
- `docs/example_serial_output.txt` — example serial output
- `docs/development_notes.md` — prototype status and next steps

## Future work
- Replace potentiometers with real RPM, temperature and current sensors.
- Calibrate and filter vibration measurements.
- Collect motor test data and validate fault thresholds.
- Implement and test a genuine fault-tolerant control strategy.
- Add logged test results and hardware photographs.

## License
This project is provided under the MIT License. See `LICENSE`.
