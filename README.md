# ESP32 Smart Device

An ESP32 project that monitors temperature, humidity, light, and motion. It
records detected events in a bounded in-memory log and provides a serial
command to search that log.

## Requirements

- Linux Mint
- Visual Studio Code
- The **PlatformIO IDE** extension for VS Code
- An ESP32 DevKit board (the project is configured for `esp32dev`)
- USB data cable and the required sensors, if running on physical hardware

PlatformIO installs the ESP32 toolchain, Arduino framework, and libraries
listed in `platformio.ini` the first time the project is built.

## Open and build

1. Install Visual Studio Code and add the **PlatformIO IDE** extension.
2. Clone or download this repository.
3. In VS Code, choose **File → Open Folder...** and select the project folder
   (the folder containing `platformio.ini`).
4. Wait for PlatformIO to initialize the project and install its dependencies.
5. In the PlatformIO toolbar, select the `esp32dev` environment and click
   **Build**. The first build may take a few minutes.

You can also open the PlatformIO home icon and use **Open Project** to select
the same folder.

## Run on an ESP32 board

1. Connect the ESP32 over USB.
2. On Linux Mint, ensure your user can access serial devices. Add your user to
   the `dialout` group if needed, then log out and back in:

   ```bash
   sudo usermod -a -G dialout "$USER"
   ```

3. In PlatformIO, click **Upload**. If the board is not detected, select its
   serial port from the PlatformIO device/port controls. Some boards require
   holding the **BOOT** button while upload begins.
4. Open **Monitor** in PlatformIO. The serial monitor baud rate is **115200**.

## Sensor connections

The pin assignments are in `include/config.hpp` and the wiring diagram is in
`diagram.json`.

| Sensor | ESP32 connection |
| --- | --- |
| DHT22 data | GPIO 4 |
| PIR output | GPIO 27 |
| Photoresistor analog output | GPIO 34 |
| Sensor power and ground | As shown in `diagram.json` |

Use the wiring diagram as the source of truth for power and ground connections.
Check the sensor and board specifications before wiring physical hardware.

## Wokwi project

For an online simulation, open the project in Wokwi and start the simulation.
Add the public Wokwi project URL to this repository description or replace
this section with a direct link once the project is published:

```text
https://wokwi.com/projects/<PROJECT_ID>
```

## Serial command

Enter a command in the serial monitor:

```text
SEARCH <TYPE> <LEVEL>
```

Supported event types: `TEMPERATURE`, `MOTION`, `LIGHT`.

Supported levels: `HIGH`, `MEDIUM`, `LOW`.

Use `*` as a wildcard for either filter, for example:

```text
SEARCH * *
SEARCH TEMPERATURE HIGH
SEARCH * LOW
```

The event log keeps up to 100 events in memory. It is cleared when the board
restarts.

## Run the native EventLogger tests

The native test only needs a C++17 compiler. On Linux Mint, install one if
necessary:

```bash
sudo apt update
sudo apt install build-essential
```

From the project directory, compile and run the test:

```bash
g++ -std=c++17 -Iinclude src/event_logger.cpp src/native_test.cpp \
    -o /tmp/esp32-smart-device-test
/tmp/esp32-smart-device-test
```

## Project layout

- `include/` — configuration, event, logger, and sensor interfaces
- `src/` — firmware implementation and native test entry point
- `test/` — EventLogger test cases
- `diagram.json` — Wokwi wiring diagram
- `platformio.ini` — ESP32 and native PlatformIO environments
