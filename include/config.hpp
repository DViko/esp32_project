#pragma once

#include <cstdint>
#include <cstddef>

/**
 * @brief Contains configuration constants for the esp32_project application.
*/
namespace Config
{
    /** @brief GPIO pin connected to the PIR motion sensor. */
    constexpr uint8_t PIR_PIN = 27;

    /** @brief GPIO pin connected to the DHT22 temperature and humidity sensor. */
    constexpr uint8_t DHT_PIN = 4;

    /** @brief GPIO pin connected to the LDR light sensor. */
    constexpr uint8_t LDR_PIN = 34;

    /** @brief Temperature threshold for generating a HIGH-level event, in Celsius. */
    constexpr float TEMPERATURE_HIGH_THRESHOLD = 30.0f;

    /** @brief ADC threshold above which the environment is considered dark. */
    constexpr int LIGHT_DARK_THRESHOLD = 2000;

    /** @brief Interval between consecutive sensor readings, in milliseconds. */
    constexpr uint32_t SENSOR_INTERVAL_MS = 2000;

    /** @brief Maximum number of events stored in the event log. */
    constexpr std::size_t MAX_EVENTS = 100;

    /** @brief Maximum number of events displayed for a single search query. */
    constexpr std::size_t MAX_DISPLAYED_EVENTS = 10;
}