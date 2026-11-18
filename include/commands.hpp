#pragma once

#include "event_logger.hpp"

#include <mutex>

void commandThreadFunc(EventLogger& logger, std::mutex& serial_mutex);