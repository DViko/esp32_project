#pragma once

#include <cstdint>

namespace Config
{
    constexpr uint8_t PIR_PIN = 27;
    constexpr uint8_t DHT_PIN = 4;
    constexpr uint8_t LDR_PIN = 34;

    constexpr float TEMPERATURE_HIGH_THRESHOLD = 30.0f;
    constexpr int LIGHT_DARK_THRESHOLD = 2000;

    constexpr uint32_t SENSOR_INTERVAL_MS = 2000;

    constexpr std::size_t MAX_EVENTS = 100;
}