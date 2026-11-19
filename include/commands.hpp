#pragma once

#include "event_logger.hpp"

#include <mutex>


/**
 * @brief Runs the command-processing thread.
 *
 * Reads commands from the Serial interface, validates their format
 * and parameters, and searches the event log when a valid SEARCH
 * command is received.
 *
 * Supported command format: SEARCH <TYPE> <LEVEL>
 *
 * @param logger Reference to the event logger used to search stored events.
 * @param serial_mutex Mutex protecting access to the Serial interface.
 */
void command_thread_func(EventLogger& logger, std::mutex& serial_mutex);