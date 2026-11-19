#pragma once

#include "event.hpp"

#include <cstddef>
#include <mutex>
#include <string>
#include <vector>

namespace Utils
{
    /**
     * @brief Prints a single message to the Serial interface.
     *
     * Acquires the provided mutex before writing to prevent output
     * from different threads from being interleaved.
     *
     * @param message Null-terminated message to print.
     * @param serial_mutex Mutex protecting access to the Serial interface.
    */
    void print_serial(const char* message, std::mutex& serial_mutex);

    /**
     * @brief Prints a newly registered event to the Serial interface.
     *
     * Displays the event type and severity level on a single line.
     * Acquires the provided mutex for the entire output operation.
     *
     * @param type Event type.
     * @param level Event severity level.
     * @param serial_mutex Mutex protecting access to the Serial interface.
    */
    void print_event(const std::string& type, const std::string& level, std::mutex& serial_mutex);

    /**
     * @brief Prints event search results to the Serial interface.
     *
     * Displays at most max_count events and holds the mutex for the
     * entire output operation to prevent interleaved messages.
     *
     * @param results Vector containing the matching events.
     * @param max_count Maximum number of results to display.
     * @param serial_mutex Mutex protecting access to the Serial interface.
    */
    void print_search_results(const std::vector<Event>& results, std::size_t max_count, std::mutex& serial_mutex);

    /**
     * @brief Returns the device uptime formatted as HH:MM:SS.
     *
     * The uptime is based on millis() and restarts after its 32-bit
     * counter overflows (approximately 49.7 days on ESP32).
     *
     * @return Uptime formatted as HH:MM:SS.
    */
    std::string get_uptime_string();
}