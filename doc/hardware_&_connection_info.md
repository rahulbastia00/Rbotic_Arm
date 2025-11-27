# Robotic Arm Hardware Connection Documentation

## Overview
This robotic arm system uses 5 servo motors controlled by an Arduino Uno through a PCA9685 PWM Servo Driver board. The system provides 5 degrees of freedom for pick-and-place operations.

---

## Hardware Components

### 1. **Microcontroller**
- **Arduino Uno** (or compatible board)
- Operating Voltage: 5V
- Digital I/O Pins: 14
- PWM Pins: 6

### 2. **Servo Driver**
- **Adafruit PCA9685 16-Channel PWM Servo Driver**
- Communication: I2C protocol
- Operating Voltage: 5V logic, can drive servos at 6V
- PWM Frequency: 50Hz (configured for standard servos)
- Channels Used: 5 out of 16 available

### 3. **Servo Motors**
Total: **5 Servo Motors** (Standard 180° servos recommended)

| Servo # | Function | Channel | Angle Range |
|---------|----------|---------|-------------|
| 1 | Base Rotation | 0 | 0° - 230° |
| 2 | Shoulder | 1 | 40° - 60° |
| 3 | Elbow | 2 | 30° - 60° |
| 4 | Wrist | 3 | 90° - 135° |
| 5 | Gripper | 4 | 0° (open) - 180° (closed) |

---

## Connection Diagram

### I2C Connection (Arduino Uno ↔ PCA9685)

```
Arduino Uno          PCA9685 Board
-----------          -------------
5V        --------->  VCC
GND       --------->  GND
A4 (SDA)  --------->  SDA
A5 (SCL)  --------->  SCL
```

### Servo Motor Connections (PCA9685 ↔ Servos)

Each servo connects to the PCA9685 board using 3 pins:
- **Signal (Yellow/White wire)** → PWM channel pin
- **Power (Red wire)** → V+ terminal
- **Ground (Brown/Black wire)** → GND terminal

```
PCA9685 Channel      Servo Motor
---------------      -----------
Channel 0    ------>  Base Servo
Channel 1    ------>  Shoulder Servo
Channel 2    ------>  Elbow Servo
Channel 3    ------>  Wrist Servo
Channel 4    ------>  Gripper Servo
```

### Power Supply

**Important:** Servos require external power supply!

```
External Power Supply (6V, 2-3A recommended)
│
├─── (+) --------> V+ on PCA9685
└─── (−) --------> GND on PCA9685
                   (Common ground with Arduino)
```

**Note:** Connect the ground of the external power supply to Arduino GND to establish a common ground reference.

---

## Pin Configuration Summary

| Component | Arduino Pin | PCA9685 Pin | Notes |
|-----------|-------------|-------------|-------|
| I2C SDA | A4 | SDA | Data line |
| I2C SCL | A5 | SCL | Clock line |
| Power (Logic) | 5V | VCC | Logic power |
| Ground | GND | GND | Common ground |
| Servo Power | External 6V | V+ | Motor power |

---

## Servo PWM Configuration

- **PWM Frequency:** 50Hz
- **Pulse Width Range:** 500µs - 2500µs
- **Resolution:** 4096 steps (12-bit)
- **Time per tick:** ~4.88µs

---

## Assembly Instructions

### Step 1: Wire the I2C Connection
1. Connect Arduino **5V** to PCA9685 **VCC**
2. Connect Arduino **GND** to PCA9685 **GND**
3. Connect Arduino **A4** to PCA9685 **SDA**
4. Connect Arduino **A5** to PCA9685 **SCL**

### Step 2: Connect Servos to PCA9685
1. Plug **Base servo** into Channel 0
2. Plug **Shoulder servo** into Channel 1
3. Plug **Elbow servo** into Channel 2
4. Plug **Wrist servo** into Channel 3
5. Plug **Gripper servo** into Channel 4

Ensure:
- Signal wires connect to the PWM pin
- Red wires connect to V+
- Black/Brown wires connect to GND

### Step 3: Connect External Power Supply
1. Connect **positive (+)** terminal to PCA9685 **V+**
2. Connect **negative (−)** terminal to PCA9685 **GND**
3. **Important:** Also connect power supply GND to Arduino GND

### Step 4: Install Required Libraries
Install via Arduino IDE Library Manager:
- `Adafruit PWM Servo Driver Library`
- `Wire.h` (included with Arduino IDE)

### Step 5: Upload Code
1. Connect Arduino Uno to computer via USB
2. Select correct board and port in Arduino IDE
3. Upload the provided code
4. Open Serial Monitor (115200 baud) to view status messages

---

## Troubleshooting

| Issue | Possible Cause | Solution |
|-------|---------------|----------|
| Servos not moving | No external power | Check 6V power supply connection |
| Jittery movement | Insufficient current | Use higher amperage power supply (3A+) |
| No I2C communication | Wrong pins | Verify A4→SDA, A5→SCL connections |
| Erratic behavior | Ground not common | Connect Arduino GND to power supply GND |

---

## Safety Notes

⚠️ **Important Safety Guidelines:**
- Never power servos directly from Arduino 5V pin
- Ensure adequate ventilation for power supply
- Use appropriate wire gauge for servo current
- Secure all connections before powering on
- Test individual servos before full assembly

---

<span style="color: red; font-weight: bold;">⚠️ NOTE: This hardware setup is initially complete. One additional hardware component needs to be set up: a CAMERA module for vision-based control. This camera integration is currently under development and will be added to the system in the next phase of the project.</span>