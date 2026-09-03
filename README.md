# Smart Door Lock System with Environmental Control & Security Alarm

An Arduino-based smart home prototype that combines password-protected door access, automatic temperature control, sound-triggered alarms, and fire detection — all built on combinational logic principles (outputs depend only on current sensor inputs, with no stored state).

## Overview

This project simulates a small smart home / smart office access and safety system using an Arduino Uno. A user enters a code on a 3x4 keypad; the system checks it, opens a servo-driven door lock, and gives color-coded LED + LCD feedback. In parallel, the system continuously monitors temperature, sound, and flame sensors to automate a cooling fan and trigger emergency alarms.

## Features

- **Password-based access control** with two access levels:
  - `1234` → Blue LED, "Welcome Dr. Momtaz", door opens
  - `5678` → Green LED, "Welcome G9", door opens
  - Wrong code → Cyan LED + buzzer, "Access Denied", door stays closed
- **Automatic temperature control** — a DC fan turns ON when the LM35 sensor reads above 40°C and OFF when it cools down
- **Sound-triggered alarm** — a clap or loud sound is detected and triggers a cyan LED + buzzer alert
- **Fire/flame detection** — a flame sensor opens the door for evacuation and sounds a continuous red-flashing alarm until the flame clears, then auto-closes after a countdown
- **16x2 I2C LCD** for real-time status messages ("Enter Code", "Access Denied", "FIRE!!!", etc.)

## Components

| Qty | Component |
|---|---|
| 1 | Arduino Uno |
| 1 | LCD 16x2 (I2C) |
| 1 | Keypad 3x4 |
| 1 | Servo Motor (door lock) |
| 1 | RGB LED |
| 1 | Buzzer |
| 1 | LM35 Temperature Sensor |
| 1 | Sound Sensor Module (LM393) |
| 1 | Flame Sensor |
| 1 | DC Fan (Motor) |
| 1 | NPN Transistor (PN2222/BC547) |
| 1 | Diode (1N4007) |
| 3 | Resistors 220Ω |
| 1 | Resistor 1kΩ |
| 1 | Breadboard |
| 1 | 9V Battery |
| 30 | Jumper Wires |

## Wiring Summary

| Signal | Arduino Pin |
|---|---|
| Keypad rows | D2, D3, D4, D5 |
| Keypad columns | D6, D7, D8 |
| LCD (I2C) | SDA → A4, SCL → A5 |
| Servo (door) | D13 |
| Red LED | D10 |
| Green LED | D11 |
| Blue LED | A3 |
| Buzzer | A1 |
| Fan (via transistor) | A0 |
| Sound sensor | D12 |
| Flame sensor | D9 |
| LM35 temperature sensor | A0 (analog input) |

See `images/arduino_schematic.png` and `images/circuit_connections.jpeg` for the full wiring diagram.

## Code

The full sketch is in [`SmartHomeSystem.ino`](./SmartHomeSystem.ino). It requires these Arduino libraries (installable via Library Manager):
- `LiquidCrystal_I2C`
- `Keypad`
- `Servo` (built-in)

## Relation to Combinational Logic

The system's outputs (door state, LED color, fan, alarms) depend only on the *current* sensor/keypad inputs at any given moment — there's no memory of past states. This is a direct hardware application of combinational logic (AND/OR/NOT conditions) for fast, real-time decision-making.

## Applications

- Multi-level access control for labs/offices
- Keyless digital security
- Thermal protection for server rooms / sensitive electronics
- Acoustic emergency alerts
- Foundation for a scalable IoT smart-home system (e.g., extendable with Wi-Fi for remote monitoring)

## Project Team

Built as an embedded systems / digital logic design project.

## License

This project is shared for educational purposes.
