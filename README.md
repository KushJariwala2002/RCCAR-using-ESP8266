# RC Car Control with ESP8266

This project allows you to control an RC car using an ESP8266 microcontroller. You can move the car forward, backward, left, right, and stop it using a web interface. The system also includes a simple interface to switch between joystick control and web control.

## Features

- **Web Interface**: A simple HTML interface that allows you to control the movement of the car.
  - **Move Forward**
  - **Move Backward**
  - **Turn Left**
  - **Turn Right**
  - **Stop**
- **Joystick Control**: A toggle to switch to joystick control, allowing you to control the car via a physical joystick (pending integration in the future).
- **Real-time Control**: You can control the car from any device connected to the same Wi-Fi network as the ESP8266.

## Components Used

- **ESP8266**: Main microcontroller for Wi-Fi and server management.
- **L298N Motor Driver**: For controlling the motors of the RC car.
- **Servo Motor**: For controlling the gate or other movements (optional).
- **Push Buttons**: For controlling the movement of the RC car via the web interface.

## Circuit Diagram

Ensure the following connections for controlling the motors and servo:

### Motor Driver (L298N):
- **IN1** (Motor 1) → D1
- **IN2** (Motor 1) → D2
- **IN3** (Motor 2) → D5
- **IN4** (Motor 2) → D6

### Servo Motor (for gate control):
- **Servo signal** → D7 (or any other available pin)

## Software Setup

### Install the required libraries:
1. **ESP8266WiFi**: For Wi-Fi connectivity.
2. **ESP8266WebServer**: To create a web server.
3. **Servo**: For controlling the servo motor (optional).

To install the libraries:
1. Open **Arduino IDE**.
2. Go to **Sketch → Include Library → Manage Libraries**.
3. Search for and install the following libraries:
   - `ESP8266WiFi`
   - `ESP8266WebServer`
   - `Servo`

### Configure Wi-Fi Credentials:

Before uploading the code, replace the Wi-Fi credentials in the code:

```cpp
const char* ssid = "YourSSID";  // Replace with your Wi-Fi SSID
const char* password = "YourPassword";  // Replace with your Wi-Fi password
