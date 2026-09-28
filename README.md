# Wireless Motor Control & Condition Monitoring System

A distributed, real-time embedded system designed for DC motor control, telemetry acquisition, and deterministic safety management. The system employs a dual-microcontroller architecture separating low-level hard real-time tasks from high-level wireless connectivity.

---

## System Architecture

The architecture delegates core motor drive, sensor sampling, and safety interlocks to an 8-bit AVR microcontroller (`ATmega32`), while utilizing a 32-bit Wi-Fi SoC (`ESP32`) as an edge telemetry gateway to stream operational data to a dashboard interface over Wi-Fi.



```markdown
# Wireless Motor Control & Condition Monitoring System

A distributed, real-time embedded system designed for DC motor control, telemetry acquisition, and deterministic safety management. The system employs a dual-microcontroller architecture separating low-level hard real-time tasks from high-level wireless connectivity.

---

## System Architecture

The architecture delegates core motor drive, sensor sampling, and safety interlocks to an 8-bit AVR microcontroller (`ATmega32`), while utilizing a 32-bit Wi-Fi SoC (`ESP32`) as an edge telemetry gateway to stream operational data to a dashboard interface over Wi-Fi.


```

+-------------------+        UART        +--------------------------+        Wi-Fi        +--------------------+
|                   |  (5V TTL / 115k2)  |  2-Ch Logic Level Conv.  |    (TCP/UDP/HTTP)   |  Remote Dashboard  |
|     ATmega32      |<------------------>|   (TX/RX Level Shift)    |<------------------->| (PC / Monitoring)  |
| (Core Controller) |                    |     5V <---> 3.3V        |                     +--------------------+
+---------+---------+                    +------------+-------------+
|                                           |
|                                    +------+------+
|                                    |    ESP32    |
|                                    |  (Gateway)  |
|                                    +-------------+
|
+-------+-------------------------+-------------------------+
|                                 |                         |
v                                 v                         v
+------------------+     +--------------------+     +-------------------+
| Actuators & Safety|     | Condition Feedback |     | Power System      |
|------------------|     |--------------------|     |-------------------|
| L298N H-Bridge   |     | Quadrature Encoder |     | 12V DC Supply     |
| 210 RPM DC Motor |     | MPU6050 (I2C Gyro) |     | LM2596 Step-Down  |
| HC-SR04 Ultrasonic|    | Dual NTC 10K ADC   |     | Rail Decoupling   |
| Active Buzzer    |     | Bus Voltage Sense  |     | Overcurrent Fuse  |
+------------------+     +--------------------+     +-------------------+

```

---

## Key Hardware Specifications

* **Primary Controller:** Microchip ATmega32 (8-bit AVR, running @ 16 MHz)
* **Wireless Gateway:** ESP32 NodeMCU (32-bit Dual-Core Xtensa LX6)
* **Actuator:** 25GA370 12V DC Gear Motor (210 RPM nominal speed)
* **Motor Driver:** L298N Dual H-Bridge Module (PWM speed control + directional IO)
* **Level Shifting:** 2-Channel Bidirectional MOSFET Logic Level Converter (5V ATmega32 $\leftrightarrow$ 3.3V ESP32)
* **Sensory Network:**
  * **Speed Feedback:** Integrated Optical/Hall Quadrature Encoder
  * **Vibration/Motion:** MPU6050 (6-DOF IMU via I2C)
  * **Thermal Monitoring:** Dual 10K NTC Thermistors (Motor housing & Power electronics)
  * **Safety Interlock:** HC-SR04 Ultrasonic Distance Sensor
  * **Bus Voltage:** Analog Voltage Divider Module (Scaled to ADC reference range)

---

## Core Firmware Features & Safety Logic

### 1. Hard Real-Time Motor Control & Sensing
* **Speed Regulation & Measurement:** Quadrature encoder pulses are captured using hardware interrupts and timer counters to calculate instantaneous RPM.
* **Closed-Loop Evaluation:** Measured RPM is continuously cross-referenced against the commanded setpoint.
* **Multi-Channel ADC Sampling:** Periodically reads system voltage and thermal states from dual NTC probes.

### 2. Deterministic Safety & Fault Detection
* **Obstacle Proximity Protection:** Ultrasonic sensor monitors ahead of the mechanical load. Crossing a predefined threshold triggers an immediate hard-stop sequence and asserts an audible alarm via the active buzzer.
* **Under-Speed / Stall Fault Detection:** If measured RPM drops below a configurable percentage of the commanded speed for a specified time window $\Delta t$, the system flags a speed loss fault, triggers visual/audible alarms, and transmits telemetry error flags.

### 3. Inter-Chip Telemetry Pipeline
* Full duplex UART packet transmission from ATmega32 to ESP32.
* Level converter preserves signal integrity across power domains (5V / 3.3V).
* ESP32 handles Wi-Fi connection management and packages sensor telemetry for laptop consumption.

---

## Safety & State Logic Summary

| Operational Condition | ATmega32 Response | Buzzer Output | Motor State |
| :--- | :--- | :--- | :--- |
| **Normal Operation** | Closed-loop RPM monitoring & telemetry streaming | OFF | RUN (Commanded PWM) |
| **Obstacle Detected** | Asserts safety-stop condition; latches error state | Intermittent / Continuous Warning | STOP (Immediate) |
| **Unexpected RPM Drop**| Flags load fault; transmits telemetry error frame | Active Fault Alarm | Controlled Safety State |

---

## System Power Distribution & Signal Conditioning

* **12V Main Rail:** Supplies raw power directly to the L298N H-bridge for motor driving.
* **5V Step-Down Rail:** Derived via LM2596 buck regulator for ATmega32 logic and 5V sensor peripherals.
* **3.3V Regulation:** Clean low-noise power rail for ESP32 and digital sensors.
* **Filtering & Decoupling:**
  * Bulk decoupling ($470\mu\text{F} - 1000\mu\text{F}$) near high-current switching paths to suppress voltage sags.
  * $100\text{nF}$ ceramic bypass capacitors placed locally across digital power pins for high-frequency noise rejection.
  * Overcurrent protection via inline fuse assembly.

---

## Directory Structure

```text
.
├── APP/             # Application layer (State machines, Control tasks)
├── HAL/             # Hardware Abstraction Layer (Motor, Sensors, Buzzer)
├── MCAL/            # Microcontroller Abstraction Layer (DIO, ADC, Timers, UART, I2C)
├── LIB/             # Utility headers, macros, bit math operations
├── build/           # Compilation output & HEX artifacts
└── README.md        # Project documentation

```

```

```