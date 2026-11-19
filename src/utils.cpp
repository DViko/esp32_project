#include "utils.hpp"

#include <Arduino.h>

#include <algorithm>
#include <cstdio>

namespace Utils
{
    void print_serial(const char* message, std::mutex& serial_mutex)
    {
        std::lock_guard<std::mutex> lock(serial_mutex);
        Serial.println(message);
    }

    void print_event(const std::string& type, const std::string& level, std::mutex& serial_mutex)
    {
        std::lock_guard<std::mutex> lock(serial_mutex);
        Serial.print("New event: ");
        Serial.print(type.c_str());
        Serial.print(" | ");
        Serial.println(level.c_str());
    }

    void print_search_results(const std::vector<Event>& results, std::size_t max_count, std::mutex& serial_mutex)
    {
        std::lock_guard<std::mutex> lock(serial_mutex);

        Serial.println("Search results:");

        const std::size_t count = std::min(results.size(), max_count);

        for (std::size_t i = 0; i < count; ++i)
        {
            Serial.print(results[i].time.c_str());
            Serial.print(" | ");
            Serial.print(results[i].type.c_str());
            Serial.print(" | ");
            Serial.println(results[i].level.c_str());
        }
    }

    std::string get_uptime_string()
    {
        // millis() overflows after approximately 49.7 days on ESP32.
        // As a result, the reported uptime starts over after the overflow.
        const unsigned long uptime_ms = millis();
        const unsigned long uptime_s = uptime_ms / 1000;
        const unsigned long hours = uptime_s / 3600;
        const unsigned long minutes = (uptime_s % 3600) / 60;
        const unsigned long seconds = uptime_s % 60;

        char buffer[16];
        snprintf(buffer, sizeof(buffer), "%02lu:%02lu:%02lu", hours, minutes, seconds);

        return std::string(buffer);
    }
}