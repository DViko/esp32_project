#include "commands.hpp"
#include "config.hpp"
#include "utils.hpp"

#include <Arduino.h>

#include <string>
#include <vector>


void command_thread_func(EventLogger& logger, std::mutex& serial_mutex)
{
    Utils::print_serial("Command thread started.", serial_mutex);

    while (true)
    {
        if (Serial.available() > 0)
        {
            // Read and normalize the incoming command.
            String input = Serial.readStringUntil('\n');

            input.trim();
            input.toUpperCase();

            // Extract the command name, event type, and severity level.
            const int first_space = input.indexOf(' ');
            const int second_space = input.indexOf(' ', first_space + 1);

            if (first_space < 0 || second_space < 0)
            {
                Utils::print_serial("Invalid command format.", serial_mutex);
                continue;
            }

            const String command = input.substring(0, first_space);
            const String type = input.substring(first_space + 1, second_space);

            const String level = input.substring(second_space + 1);

            if (command != "SEARCH" || type.isEmpty() || level.isEmpty() || level.indexOf(' ') >= 0)
            {
                Utils::print_serial("Invalid command.", serial_mutex);
                continue;
            }

            const std::string event_type(type.c_str());
            const std::string event_level(level.c_str());

            // Validate the requested event type and severity level.
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
                Utils::print_serial("Invalid event type or level.", serial_mutex);
                continue;
            }

            // Search the event log using the validated parameters.
            const std::vector<Event> results = logger.search_events(event_type, event_level);

            if (results.empty())
            {
                Utils::print_serial("No matching events found.", serial_mutex);
                continue;
            }

            Utils::print_search_results(results, Config::MAX_DISPLAYED_EVENTS, serial_mutex);
        }

        delay(10);
    }
}