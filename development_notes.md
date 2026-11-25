# Development notes

## Current prototype
- STM32 NUCLEO-C031C6 firmware reads three potentiometer inputs.
- Potentiometers simulate RPM, temperature and current.
- MPU6050 acceleration differences provide a simple motion indicator.
- Serial Monitor reports NORMAL, WARNING or CRITICAL.
- Buzzer is activated for CRITICAL.

## Limitations
- Scaling values and thresholds are illustrative.
- The vibration indicator is an uncalibrated sum of raw acceleration changes.
- No real motor, marine propulsion unit or fault-tolerant controller has yet been validated.
- Confirm buzzer polarity and electrical requirements for the exact module being used.

## Next development steps
1. Replace simulated inputs with appropriate sensors.
2. Calibrate sensor conversions.
3. Filter and characterize vibration data.
4. Run controlled motor fault experiments.
5. Validate thresholds and implement a tested safe-state control strategy.
