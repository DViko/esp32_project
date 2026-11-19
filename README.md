# 📝 ESP32 Project

A sensor-based smart device simulation built with ESP32, C++17, and Arduino Framework.

The project demonstrates concurrent programming, sensor monitoring, event logging, thread synchronization, and command-based event searching.

The application runs in the Wokwi ESP32 simulator and uses two independent threads to handle sensor monitoring and user commands.


## 📝 Live Simulation

The project is available as an interactive simulation on Wokwi.

**[Open ESP32_Project in Wokwi](https://wokwi.com/projects/481024215761310721)**

You can start the simulation, interact with the virtual sensors, and test the application's functionality through the Serial Monitor.

No local installation is required to try the online simulation.


## 📝 Features

- **Sensor monitoring:** Reads data from a DHT22 temperature and humidity sensor, a PIR motion sensor, and a photoresistor.
- **Multithreading:** Uses std::thread to run sensor monitoring and command processing concurrently.
- **Thread synchronization:** Uses std::mutex to protect shared event data and serial output.
- **Event logging:** Stores detected events in a thread-safe event log.
- **FIFO storage:** Keeps up to 100 events. The oldest event is removed when the log reaches its capacity.
- **Event searching:** Supports filtering by event type and severity level.
- **Event prioritization:** Search results are sorted by severity: HIGH, MEDIUM, then LOW.
- **Input validation:** Checks user commands before processing them.


## 📝 Requirements

The project was developed and tested on Linux Mint (Ubuntu-based).

🔸 Required software:

  - Visual Studio Code.
  - PlatformIO IDE extension for VS Code.
  - Wokwi Simulator extension for VS Code.

PlatformIO automatically installs the required ESP32 platform and library dependencies during the first build.


## 📝 Technologies

  - C++17
  - ESP32
  - Arduino Framework
  - PlatformIO
  - Wokwi
  - C++ Standard Library: std::thread, std::mutex, std::vector


## 📝 Hardware

🔸 The project uses the following components and connections in the Wokwi simulation:

### Components

| Component           | Purpose                             |
| :------------------ | :---------------------------------- |
| ESP32 DevKit        | Main controller                     |
| DHT22               | Temperature and humidity monitoring |
| PIR Motion Sensor   | Motion detection                    |
| Photoresistor (LDR) | Light level monitoring              |


### GPIO Connections

| Component | ESP32 GPIO |
| :-------- | :--------- |
| PIR OUT   | GPIO 27    |
| DHT22 SDA | GPIO 4     |
| LDR AO    | GPIO 34    |

Power connections are configured in diagram.json.


## 📝 Application Architecture

The application consists of two concurrent threads.

### Sensor Thread

The sensor thread periodically reads sensor data every 2 seconds.

🔸 It detects state changes and registers events when the following conditions occur:


| Event Type  | Condition                       | Severity |
| :---------- | :------------------------------ | :------- |
| TEMPERATURE | Temperature exceeds 30 °C       | HIGH     |
| MOTION      | Motion is detected              | MEDIUM   |
| LIGHT       | Ambient light level becomes low | LOW      |


Events are registered only when a monitored condition changes from inactive to active. This prevents the same event from being repeatedly added while the condition remains active.


### Command Thread

The command thread listens for commands through the Serial Monitor. It validates user input, searches the event log, and displays matching results.

**EventLogger** manages the event storage and search operations.

🔸 Its main responsibilities are:

- Thread-safe event insertion.
- FIFO event storage with a maximum capacity of 100 entries.
- Event filtering by type and severity.
- Sorting search results by severity.
- Preserving the newest-first order among events with equal severity.

The event log is protected by an internal mutex. Search operations scan the event log while holding the mutex. Matching events are copied to a result vector, and sorting happens after the mutex is released.


## 📝 Installation and Setup


### Clone the Repository

🔸 Open a terminal and run:

- > git clone https://github.com/DViko/esp32_project.git

🔸 Navigate to the project directory:

- > cd esp32_project

🔸 Open the project in Visual Studio Code:

- > code .

If you want to use this project as a starting point for your own repository, replace the repository URL with your own GitHub repository address.


### Install VS Code Extensions

🔸 Install the following extensions from the VS Code Marketplace:

- PlatformIO IDE
- Wokwi Simulator

PlatformIO manages the project dependencies and build environments. Wokwi provides the ESP32 simulation environment.


### Build the ESP32 Firmware

🔸 Open the PlatformIO: New Terminal in VS Code and run:
  - > pio run -e esp32dev

PlatformIO will download the required dependencies, compile the source files, and generate the firmware.

🔸 The firmware files are generated in:
  - > .pio/build/esp32dev/


### Start the Wokwi Simulation

🔸 After a successful build:

  - Open the project in VS Code.
  - Open the Command Palette with Ctrl + Shift + P.
  - Select Wokwi: Start Simulator.
  - Wait for the ESP32 simulation to start.
  - Open the Serial Monitor to interact with the application.

The simulation configuration is stored in wokwi.toml, while the hardware connections are defined in diagram.json.

The Serial Monitor uses a baud rate of **115200**.


## 📝 Serial Commands

🔸 The application accepts commands using the following format:

  - > SEARCH <TYPE> <LEVEL>

🔸 Available Event Types:

  - TEMPERATURE
  - MOTION
  - LIGHT

🔸 Available Severity Levels:

  - HIGH
  - MEDIUM
  - LOW

The wildcard character * can be used to match any event type or severity level.


### Examples

🔸 Search for high-temperature events:

  - > SEARCH TEMPERATURE HIGH

🔸 Search for motion events:

  - > SEARCH MOTION MEDIUM

🔸 Search for low-light events:

  - > SEARCH LIGHT LOW

🔸 Display all events:

  - > SEARCH * *

Commands are case-insensitive.

The application displays a maximum of 10 matching results per search.

Results are ordered by severity (HIGH → MEDIUM → LOW). Events with the same severity are displayed from newest to oldest.


## 📝 Native Unit Tests

The project includes a separate Native build environment for testing the EventLogger class on the host machine.

These tests do not require an ESP32 board or the Wokwi simulator.

🔸 To build and execute the tests, open the PlatformIO terminal and run:

  - > pio run -e native -t exec

The Native environment compiles the event logger and a standalone test executable.

🔸 The current test suite verifies:

  - Searching an empty event log.
  - Preserving exactly 100 events without removing any entries.
  - Enforcing the maximum event storage capacity of 100 events.
  - Removing the oldest events when the capacity is exceeded, preserving the newest entries.
  - Filtering events by type.
  - Sorting search results by severity priority (HIGH, MEDIUM, LOW), with newer events appearing first within each priority level.


## 📝 Project Status

The project has been successfully built and tested in the Wokwi ESP32 simulator.

Sensor monitoring, event registration, concurrent execution, command processing, event searching, sorting, and FIFO storage have been verified.

The Native unit test suite for EventLogger passes all tests, including FIFO capacity, event filtering, and priority sorting.

---