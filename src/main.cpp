#include <Arduino.h>

#include "sensors.hpp"
#include "event_logger.hpp"
#include "commands.hpp"

#include <mutex>
#include <thread>
#include <functional>

Sensors sensors;
EventLogger event_logger;

std::mutex serial_mutex;

void setup()
{
    Serial.begin(115200);
    delay(500);

    sensors.begin();

    std::thread sensor_thread(
        sensorThreadFunc,
        std::ref(sensors),
        std::ref(event_logger),
        std::ref(serial_mutex)
    );

    std::thread command_thread(
        commandThreadFunc,
        std::ref(event_logger),
        std::ref(serial_mutex)
    );

    sensor_thread.detach();
    command_thread.detach();
}

void loop()
{
    delay(1000);
}