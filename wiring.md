# Wiring reference

| Signal | STM32 NUCLEO-C031C6 |
|---|---|
| RPM potentiometer wiper | A0 |
| Temperature potentiometer wiper | A1 |
| Current potentiometer wiper | A2 |
| MPU6050 SDA | PB9 |
| MPU6050 SCL | PB8 |
| MPU6050 VCC | 3.3V |
| MPU6050 GND | GND |
| Buzzer signal / positive | D6 |
| Buzzer GND / negative | GND |

For each potentiometer, connect the two outer terminals to 3.3V and GND, and the centre wiper to the assigned analog pin. Use a common ground. Do not feed 5V into STM32 analog inputs or the MPU6050 supply pin.
