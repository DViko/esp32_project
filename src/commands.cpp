#include "commands.hpp"

#include <Arduino.h>

#include <vector>
#include <string>

void commandThreadFunc(EventLogger& logger, std::mutex& serial_mutex)
{
    {
        std::lock_guard<std::mutex> lock(serial_mutex);
        Serial.println("Command thread started.");
        Serial.println("Use: SEARCH <TYPE> <LEVEL>");
    }

    while (true)
    {
        if (Serial.available() > 0)
        {
            String input = Serial.readStringUntil('\n');

            input.trim();
            input.toUpperCase();

            const int first_space = input.indexOf(' ');
            const int second_space = input.indexOf(' ', first_space + 1);

            if (first_space < 0 || second_space < 0)
            {
                std::lock_guard<std::mutex> lock(serial_mutex);
                Serial.println("Invalid command format.");
                Serial.println("Example: SEARCH TEMPERATURE HIGH");
                continue;
            }

            const String command = input.substring(0, first_space);
            const String type = input.substring(first_space + 1, second_space);

            const String level = input.substring(second_space + 1);

            if (command != "SEARCH" || type.isEmpty() || level.isEmpty() || level.indexOf(' ') >= 0)
            {
                std::lock_guard<std::mutex> lock(serial_mutex);
                Serial.println("Invalid command.");
                continue;
            }

            const std::string event_type(type.c_str());
            const std::string event_level(level.c_str());

            const bool valid_type
            {
                event_type == "*" ||
                event_type == "TEMPERATURE" ||
                event_type == "MOTION" ||
                event_type == "LIGHT"
            };

            const bool valid_level
            {
                event_level == "*" ||
                event_level == "LOW" ||
                event_level == "MEDIUM" ||
                event_level == "HIGH"
            };

            if (!valid_type || !valid_level)
            {
                std::lock_guard<std::mutex> lock(serial_mutex);
                Serial.println("Invalid event type or level.");
                continue;
            }

            const std::vector<Event> results = logger.search_events(event_type, event_level);

            std::lock_guard<std::mutex> lock(serial_mutex);

            if (results.empty())
            {
                Serial.println("No matching events found.");
                continue;
            }

            Serial.println("Search results:");

            const std::size_t count = results.size() < 10 ? results.size() : 10;

            for (std::size_t i = 0; i < count; ++i)
            {
                Serial.print(results[i].time.c_str());
                Serial.print(" | ");
                Serial.print(results[i].type.c_str());
                Serial.print(" | ");
                Serial.println(results[i].level.c_str());
            }
        }

        delay(10);
    }
}