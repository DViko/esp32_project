#include "sensors.hpp"
#include "config.hpp"
#include "event_logger.hpp"

#include <Arduino.h>
#include <cstdio>
#include <string>

namespace
{
std::string get_uptime_string()
{
    const uint32_t total_seconds = millis() / 1000;
    const uint32_t hours = total_seconds / 3600;
    const uint32_t minutes = (total_seconds % 3600) / 60;
    const uint32_t seconds = total_seconds % 60;

    char buffer[16];
    snprintf(
        buffer,
        sizeof(buffer),
        "%02lu:%02lu:%02lu",
        static_cast<unsigned long>(hours),
        static_cast<unsigned long>(minutes),
        static_cast<unsigned long>(seconds)
    );

    return std::string(buffer);
}
}

void sensorThreadFunc(
    Sensors& sensors,
    EventLogger& logger,
    std::mutex& serial_mutex
)
{
    // Keep the previous readings so events are generated on state changes.
    bool digital_states_initialized = false;
    bool temperature_state_initialized = false;
    bool previous_motion = false;
    bool previous_dark = false;
    bool previous_high_temperature = false;

    {
        std::lock_guard<std::mutex> lock(serial_mutex);
        Serial.println("Sensor thread started.");
    }

    while (true)
    {
        const SensorData data = sensors.read_sensors();

        const auto register_event =
            [&](const std::string& type, const std::string& level)
        {
            Event event;
            event.time = get_uptime_string();
            event.type = type;
            event.level = level;

            logger.add_event(event);

            std::lock_guard<std::mutex> lock(serial_mutex);
            Serial.print("New event: ");
            Serial.print(type.c_str());
            Serial.print(" | ");
            Serial.println(level.c_str());
        };

        if (data.dht_valid)
        {
            const bool high_temperature =
                data.temperature > Config::TEMPERATURE_HIGH_THRESHOLD;

            if (temperature_state_initialized &&
                !previous_high_temperature &&
                high_temperature)
            {
                register_event("TEMPERATURE", "HIGH");
            }

            previous_high_temperature = high_temperature;
            temperature_state_initialized = true;
        }

        const bool is_dark =
            data.light_level > Config::LIGHT_DARK_THRESHOLD;

        if (digital_states_initialized)
        {
            if (!previous_motion && data.motion_detected)
            {
                register_event("MOTION", "MEDIUM");
            }

            if (!previous_dark && is_dark)
            {
                register_event("LIGHT", "LOW");
            }
        }
        else
        {
            digital_states_initialized = true;
        }

        previous_motion = data.motion_detected;
        previous_dark = is_dark;

        delay(Config::SENSOR_INTERVAL_MS);
    }
}
