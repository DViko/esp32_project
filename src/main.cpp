#include <Arduino.h>

#include "sensors.hpp"
#include "event_logger.hpp"
#include "commands.hpp"

#include <mutex>
#include <thread>
#include <functional>

/**
 * @brief Shared sensor interface used by the sensor monitoring thread.
 */
Sensors sensors;

/**
 * @brief Shared event logger used by both application threads.
 */
EventLogger event_logger;

/**
 * @brief Mutex protecting concurrent access to the Serial interface.
 */
std::mutex serial_mutex;

/**
 * @brief Initializes the device and starts application threads.
 *
 * Initializes Serial communication and sensors, then creates
 * and detaches the sensor monitoring and command processing threads.
 */
void setup()
{
    Serial.begin(115200);
    delay(500);

    sensors.begin();

    std::thread sensor_thread(
        sensor_thread_func,
        std::ref(sensors),
        std::ref(event_logger),
        std::ref(serial_mutex)
    );

    std::thread command_thread(
        command_thread_func,
        std::ref(event_logger),
        std::ref(serial_mutex)
    );

    sensor_thread.detach();
    command_thread.detach();
}

/**
 * @brief Arduino main loop.
 *
 * Application logic runs in separate threads.
 * The delay prevents unnecessary CPU usage by the main loop.
 */
void loop()
{
    delay(1000);
}