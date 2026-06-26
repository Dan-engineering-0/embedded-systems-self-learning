# Experiment 20: ESP32 3D Radar Scanner (Single-Core Prototype)

## Overview
This repository contains the first working prototype of a 3D environmental scanner. The system utilizes an ESP32 microcontroller to orchestrate a mechanical scanning tower while simultaneously hosting a web server to display real-time spatial data (distance, pan, and tilt angles) on a local network.

Currently, this is a **Single-Core / Blocking** implementation, serving as a proof-of-concept for the hardware mechanics and the client-server data pipeline.

## Hardware Stack
* **Microcontroller:** ESP32 Development Board
* **Distance Sensor:** HC-SR04 Ultrasonic Sensor (with a voltage divider for the 5V Echo to 3.3V GPIO).
* **Pan Axis (Horizontal):** 28BYJ-48 Stepper Motor with ULN2003 Driver.
* **Tilt Axis (Vertical):** SG90 Micro Servo Motor.
* **Power:** MB102 Breadboard Power Supply Module (5V/3.3V) to isolate motor current spikes from the ESP32.

## Visuals
*(Images of the current setup and the web interface)*

![System Hardware Setup](hardware_setup.jpg)
*Caption: The physical scanning tower, showing the stepper base, servo tilt mechanism, and the isolated power routing on the breadboard.*

![Web Dashboard](browser_dashboard.png)
*Caption: The responsive HTML/JS dashboard hosted on the ESP32, fetching live JSON data (Pan, Tilt, Distance).*

## Current Architecture & Achievements
1.  **Hardware-Software Integration:** Successfully synchronized the stepper motor (sweeping motion) and servo (elevation steps).
2.  **Web Server:** Configured the ESP32 to connect to a local WiFi network and host an embedded HTML page.
3.  **Data Pipeline:** The ESP32 serves a JSON endpoint (`/data`), which is polled by the browser using JavaScript `fetch()` API.
4.  **Hardware Debugging:** Addressed critical ESP32 boot failures when running on external power:
    * Avoided `GPIO 12` (Strapping Pin for Flash Voltage) which caused bootloops when pulled high by the ULN2003 driver.
    * Avoided `GPIO 5` (Strapping Pin) for the Ultrasonic Trigger.
    * Established common ground (GND) without backfeeding 5V to the USB host.

## Current Limitations (The "Single-Core" Problem)
In this version, all tasks are running sequentially on a single core (Core 1) using standard Arduino functions:
* `pulseIn()` blocks the CPU while waiting for the ultrasonic echo.
* WiFi handling and client HTTP requests interrupt the motor timing, causing slight jitters.
* Stepper motor steps are executed using blocking delays, preventing true real-time sensor reading.

## Next Steps & Roadmap
To transition this from a basic prototype to an optimized, real-time embedded system, the following upgrades are planned for the next iterations:

* [ ] **FreeRTOS Dual-Core Implementation:** Pin the WiFi stack and Web Server to Core 0, isolating all hardware control (Motors & Sensors) to Core 1 for jitter-free execution.
* [ ] **Interrupt-Driven Sensors:** Replace the blocking `pulseIn()` with Hardware Timers and Interrupt Service Routines (ISRs) to measure the HC-SR04 echo pulse without halting the CPU.
* [ ] **Logic Analyzer Integration:** Utilize a USB Logic Analyzer to monitor SPI/I2C buses and PWM timings, ensuring the WiFi stack isn't injecting latency into the hardware signals.
* [ ] **Mechanical Upgrade:** Integrate a Lazy Susan bearing to bear the structural load, relieving the stepper motor shaft.
* [ ] **3D Point Cloud Rendering:** Upgrade the web client from basic text display to a Three.js 3D point cloud visualization using WebSockets for lower latency.
