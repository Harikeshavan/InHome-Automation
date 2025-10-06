# IoT Home Automation using ESP8266

An ESP8266-based IoT home automation system that enables remote control and automation of home appliances such as a bulb and fan. The system hosts its own Wi-Fi access point and web server, allowing users to control devices via a browser interface in Auto or Manual mode.

---

## Features

- **Wi-Fi Access Point** – The ESP8266 creates its own hotspot for local access.  
- **Web Server Interface** – Control appliances using any device with a web browser.  
- **Dual Modes:**  
  - **Auto Mode** – Sensors manage devices automatically based on environment.  
  - **Manual Mode** – User controls each device through the webpage.  
- **Sensor Integration:**  
  - **DHT11** – Monitors ambient temperature.  
  - **LDR** – Detects light intensity for bulb control.  
  - **Ultrasonic Sensor** – Detects presence and counts people.  
- **Real-Time Feedback** – Displays current device status and mode.

---

## Hardware Components

| Component | Description |
|------------|-------------|
| ESP8266 NodeMCU | Main microcontroller with Wi-Fi |
| DHT11 Sensor | Measures ambient temperature |
| LDR | Detects ambient light intensity |
| Ultrasonic Sensor (HC-SR04) | Measures distance and detects human presence |
| Relay Modules | Controls bulb and fan |
| Bulb & Fan | Output devices |

---

## Working Principle

The ESP8266 runs a web server accessible through a Wi-Fi network (SSID: **ESP_8266**, Password: **1234567890**).

- In **Auto Mode**, the system reads sensor data:  
  - If light is low, the bulb turns ON.  
  - If temperature > 27°C, the fan turns ON.  
  - Ultrasonic sensor increments the people count when motion is detected.  
- In **Manual Mode**, users can toggle devices via the webpage.

---

## Web Interface

The web page displays:  
- Mode of operation (Auto / Manual)  
- Bulb and Fan status  
- Control buttons for switching ON/OFF  
- A clean and responsive UI styled with embedded CSS

---

## Code Overview

- **setup()** – Initializes pins, Wi-Fi AP, and web server routes.  
- **loop()** – Continuously reads sensors, updates states, and handles client requests.  
- **Handlers** – Manage requests like `/bulbon`, `/fanoff`, `/modeauto`, etc.  
- **htmlpg()** – Dynamically generates the HTML interface with current system states.

---

## Pin Configuration

| Device | GPIO Pin | NodeMCU Pin |
|---------|-----------|-------------|
| Bulb | 15 | D8 |
| Fan | 13 | D7 |
| LDR | 14 | D5 |
| Ultrasonic Trigger | 12 | D6 |
| Ultrasonic Echo | 5 | D1 |
| DHT11 | 2 | D4 |
