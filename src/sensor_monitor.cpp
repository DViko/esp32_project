#include "sensors.hpp"
#include "config.hpp"
#include "event_logger.hpp"
#include "utils.hpp"

#include <Arduino.h>

#include <mutex>
#include <string>

void sensor_thread_func(Sensors& sensors, EventLogger& logger, std::mutex& serial_mutex)
{
    // Track previous sensor states to detect state transitions.
    bool digital_states_initialized = false;
    bool temperature_state_initialized = false;
    bool previous_motion = false;
    bool previous_dark = false;
    bool previous_high_temperature = false;

    Utils::print_serial("Sensor thread started.", serial_mutex);

    while (true)
    {
        const SensorData data = sensors.read_sensors();

        const auto register_event = [&](const std::string& type, const std::string& level)
        {
            Event event;
            event.time = Utils::get_uptime_string();
            event.type = type;
            event.level = level;

            logger.add_event(event);

            Utils::print_event(type, level, serial_mutex);
        };

        // Process temperature only when both DHT readings are valid.
        if (data.dht_valid)
        {
            const bool high_temperature = data.temperature > Config::TEMPERATURE_HIGH_THRESHOLD;

            // Register an event only when temperature crosses the configured threshold.
            if (temperature_state_initialized && !previous_high_temperature && high_temperature)
            {
                register_event("TEMPERATURE", "HIGH");
            }

            previous_high_temperature = high_temperature;
            temperature_state_initialized = true;
        }

        const bool is_dark = data.light_level > Config::LIGHT_DARK_THRESHOLD;

        // Detect rising edges after the initial sensor states have been established.
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
