#pragma once

#include "config.hpp"
#include "event.hpp"

#include <vector>
#include <mutex>

class EventLogger
{
    private:
        static constexpr std::size_t MAX_EVENTS = Config::MAX_EVENTS;

        std::vector<Event> events;

        mutable std::mutex event_mutex;

    public:
        void add_event(const Event& event);

        std::vector<Event> search_events(const std::string& type, const std::string& level) const;
};