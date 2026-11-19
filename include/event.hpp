#pragma once

#include <string>

/**
* @brief Represents an event detected by the esp32_project application.
*/
struct Event
{
    /** @brief Time elapsed since application startup, formatted as HH:MM:SS. */
    std::string time;

    /** @brief Type of the detected event (TEMPERATURE, MOTION, or LIGHT). */
    std::string type;

    /** @brief Severity level of the event (LOW, MEDIUM, or HIGH). */
    std::string level;
};