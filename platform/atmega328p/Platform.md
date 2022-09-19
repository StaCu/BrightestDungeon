# ATMEGA328P

Most of the pins have been hardcoded (but the assembler code can of course be changed).
The default values are:

Usage              | Pin
-------------------|---------------
Simple Controller  |
\|-- Potentiometer  | A5
\|-- Attack Button  | D2
MPU6050            |
\|-- SCL            | A5
\|-- SDA            | A4
\|-- Attack Button  | D2
LED                |
\|-- Data           | D8
Sound              |
\|-- Passive Buzzer | D9, D10
Options            | D3
SD Card            | (many sdcard modules use 3.3V)
\|-- MOSI           | D11
\|-- MISO           | D12
\|-- CLK            | D13
\|-- CS             | D4


## Required Libraries:

- Common
  - SdFat
- Specific
  - I2Cdev
  - MPU6050
  - Platform
