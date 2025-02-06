# Bicycle Safety and Navigation System

A project aimed at enhancing bicycle safety and navigation through automation and real-time data display.

## Overview

This project integrates multiple sensors and components to provide cyclists with real-time navigation assistance and safety features. The system utilizes the following components:

- **MPU6050**: A 6-axis motion tracking device combining a 3-axis gyroscope and a 3-axis accelerometer.
- **NEO-6M GPS Module**: Provides real-time geographic positioning data.
- **RFID Sensor**: Enables identification and authentication functionalities.
- **SG90 Servo Motor**: Used to lock and unlock the bicycle.
- **0.96-inch OLED Display**: Displays information such as speed, location, and system status to the rider.

## Features

- **Real-Time Navigation**: Utilizes the NEO-6M GPS module to provide current location data to the rider.
- **Motion Monitoring**: Employs the MPU6050 sensor to monitor the bicycle's motion, potentially detecting unsafe riding conditions.
- **User Authentication**: Integrates an RFID sensor to ensure that only authorized users can access certain features or functionalities.
- **Bicycle Locking Mechanism**: Uses the SG90 servo motor to lock and unlock the bicycle based on authentication inputs.
- **Information Display**: The OLED display presents real-time data, including speed and navigation cues, to the cyclist.

## Repository Structure

The repository is organized into the following directories:

- **Blinkers**: Code related to the bicycle's indicator system.
- **Dynamic\_Swipe\_Pattern**: Pattern of indicators.
- **GPS**: Scripts and configurations for GPS data acquisition and processing.
- **GPS\_with\_OLED**: Integration of GPS data display on the OLED screen.
- **Main**: The primary codebase for the system's core functionalities.
- **MaterESP**: ESP microcontroller-related configurations and code.
- **RTOS**: Real-Time Operating System implementations for task management.
- **Speed**: Modules related to speed calculation and monitoring.

## Getting Started

To set up and run the Bicycle Safety and Navigation System:

1. **Hardware Setup**:

   - Connect the MPU6050, NEO-6M GPS module, RFID sensor, SG90 servo motor, and 0.96-inch OLED display to the microcontroller as per the wiring schematics provided in the respective directories.

2. **Software Installation**:

   - Clone this repository to your local machine.
   - Ensure you have the necessary libraries installed for each component (e.g., Wire.h for I2C communication, Adafruit\_GFX.h and Adafruit\_SSD1306.h for the OLED display).

3. **Compilation and Upload**:

   - Open the main project file in your preferred IDE (e.g., Arduino IDE).
   - Compile and upload the code to the microcontroller.

4. **Operation**:

   - Upon powering the system, the OLED display will show real-time data such as current speed and location.
   - The system will monitor motion parameters and provide alerts or take actions as programmed.
   - The bicycle locking mechanism will be controlled using RFID authentication and the SG90 servo motor.

## Contributing

Contributions to enhance the system's features, improve code efficiency, or expand documentation are welcome. Please fork the repository and submit a pull request with your proposed changes.
