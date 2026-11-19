#pragma once

#include "config.hpp"
#include "event.hpp"

#include <vector>
#include <mutex>

/**
* @brief Stores, manages, and searches application events.
*
* The event log has a fixed maximum capacity.
* When the capacity is reached, the oldest event is removed before a new event is added.
* All access to the internal event collection is protected by a mutex, allowing safe access from multiple threads.
*/
class EventLogger
{
    public:
        /**
        * @brief Adds an event to the log. If the log is full, the oldest event is removed first.
        * @param event Event to be stored.
        */
        void add_event(const Event& event);

        /**
         * @brief Searches the event log using event type and severity level.
         *
         * The wildcard "*" matches any event type or severity level.
         * Results are sorted by severity (HIGH, MEDIUM, LOW).
         * Events with equal severity retain reverse chronological order,
         * with newer events appearing first.
         *
         * @param type Event type to search for, or "*" to match all types.
         * @param level Severity level to search for, or "*" to match all levels.
         * @return A vector containing the matching events in sorted order.
        */
        std::vector<Event> search_events(const std::string& type, const std::string& level) const;

    private:
        /** @brief Maximum number of events retained in the log. */
        static constexpr std::size_t MAX_EVENTS = Config::MAX_EVENTS;

        /** @brief Collection of stored events in chronological insertion order. */
        std::vector<Event> events;

        /** @brief Mutex protecting access to the internal event collection. */
        mutable std::mutex event_mutex;
};