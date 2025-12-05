# pic16f877a-fire-smoke-alarm-lm35-exhaust-fan
Fire and smoke over-temperature alarm using PIC16F877A, LM35, MQ-2 and exhaust fan relay (CCS C)

# Fire and Smoke Over-Temperature Alarm with Exhaust Fan using PIC16F877A (CCS C)

This is a small embedded project using a **PIC16F877A** microcontroller and **CCS C** compiler to monitor temperature and gas/smoke level and control an **exhaust fan** via a relay.  
The system reads temperature from an **LM35 sensor** through the ADC, checks for gas/smoke using an **MQ-2 gas sensor**, displays information and status on a **16x2 LCD**, and activates a **buzzer** and **exhaust fan relay** when dangerous conditions are detected. :contentReference[oaicite:1]{index=1}

## Features

- Measure ambient **temperature (°C)** using an **LM35** analog temperature sensor.
- Display live temperature on a **16x2 character LCD**.
- Detect gas/smoke using an **MQ-2** gas sensor (digital output).
- Combine **over-temperature** and **gas** conditions:
  - Normal: temperature below threshold and no gas.
  - Over-temperature only.
  - Gas only.
  - Both over-temperature and gas at the same time.
- Use a **buzzer** for audible alarm.
- Drive an **exhaust fan** (or alarm device) through a **relay output**:
  - Relay ON in any abnormal condition (over-temperature and/or gas).
  - Relay OFF in normal condition.
- Show clear status messages on the LCD (normal / over-temperature / gas / both).

## Hardware

- Microcontroller: **PIC16F877A** (20 MHz external crystal)
- Temperature sensor: **LM35** (connected to ADC channel AN0)
- Gas/smoke sensor: **MQ-2** gas sensor module (digital output on RA1)
- 16x2 character LCD (4-bit mode) using `lcd.c` library
- Relay module to drive the exhaust fan (connected to `PIN_C6`)
- Buzzer (connected to `PIN_D2`)
- Power supply (5 V for logic and sensors, separate supply for fan if needed)
- Miscellaneous: resistors, wires, breadboard or PCB

Typical pin usage in the example code:

- LM35 analog input: `AN0` (ADC channel 0)
- MQ-2 digital output: `PIN_A1`
- Relay (exhaust fan): `PIN_C6`
- Buzzer: `PIN_D2`
- LCD:
  - `LCD_ENABLE_PIN` → `PIN_B2`
  - `LCD_RS_PIN`     → `PIN_B4`
  - `LCD_RW_PIN`     → `PIN_B3`
  - `LCD_DATA4`      → `PIN_D7`
  - `LCD_DATA5`      → `PIN_D6`
  - `LCD_DATA6`      → `PIN_D5`
  - `LCD_DATA7`      → `PIN_D4`

## Software & Tools

- **Compiler**: CCS C for PIC (`#include <16F877A.h>`)  
- Clock: 20 MHz (`#use delay(clock = 20000000)`)
- LCD library: `lcd.c` (standard CCS LCD driver, 4-bit mode)
- IDE: CCS IDE or MPLAB X with CCS plug-in
- Programmer: PICkit or compatible programmer for PIC16F877A
- Optional: Proteus for circuit simulation and testing

## Code Overview

Main source file: `fire_smoke_fan_alarm.c`

Key points:

- **LM35 temperature reading**:
  - ADC is configured on channel AN0.
  - The function `doc_lm35()` takes 100 samples, sums them and computes an average to reduce noise.
  - The averaged ADC value is converted to millivolts and then to °C:
    - `LM35` has 10 mV/°C, with a 5 V reference and 10-bit ADC.
  - The result (integer temperature) is stored in `nd` and shown on the LCD as `"NHIET DO:xxC"`.
- **MQ-2 gas detection**:
  - MQ-2 digital output is read from `mq2` (defined as `PIN_A1`).
  - In this design, a low level (`input(mq2) == 0`) indicates gas/smoke detected (depending on the module’s configuration).
- **Relay and buzzer control**:
  - `loa` (buzzer) is on `PIN_D2`.
  - `relay` (exhaust fan relay) is on `PIN_C6`.
  - In any alarm condition, both buzzer and relay are turned ON.
  - In normal condition, both are turned OFF.
- **LCD status messages** (second line):
  - Over-temperature + gas: `"QUA NHIET-KHI GA"`
  - Over-temperature only: `"    QUA NHIET    "`
  - Gas only: `"     KHI GA     "`
  - Normal: `"  BINH THUONG   "`

The over-temperature threshold in the code is set to **40 °C**, but it can be adjusted easily.

## How It Works (High-Level)

1. **Initialize:**
   - Configure TRIS registers:
     - Port A for sensor inputs (LM35 via ADC, MQ-2 digital).
     - Ports B and D for LCD data/control pins and buzzer.
     - Port C with relay pin as output.
   - Call `lcd_init()` to initialize the LCD.
   - Turn OFF the buzzer and relay at startup.
   - Configure the ADC for LM35:
     - `SETUP_ADC(ADC_CLOCK_DIV_32);`
     - `SETUP_ADC_PORTS(AN0);`
     - `SET_ADC_CHANNEL(0);`

2. **In the main loop:**
   - Call `doc_lm35()`:
     - Read and average 100 ADC samples.
     - Convert to temperature in °C and display it on the first line of the LCD.
   - Read MQ-2 digital output using `input(mq2)` (RA1).
   - Evaluate alarm conditions:
     - If temperature > 40 °C **and** gas detected:
       - Turn ON buzzer and relay.
       - Show `"QUA NHIET-KHI GA"` on the second LCD line.
     - Else if temperature > 40 °C only:
       - Turn ON buzzer and relay.
       - Show `"    QUA NHIET    "`.
     - Else if gas detected only:
       - Turn ON buzzer and relay.
       - Show `"     KHI GA     "`.
     - Else (no over-temperature and no gas):
       - Turn OFF buzzer and relay.
       - Show `"  BINH THUONG   "`.
   - Repeat this loop continuously to monitor the environment and control the exhaust fan and alarm in real time.
