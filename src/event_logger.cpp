#include "event_logger.hpp"

#include <algorithm>

void EventLogger::add_event(const Event& event)
{
    std::lock_guard<std::mutex> lock(event_mutex);

    if (events.size() >= MAX_EVENTS)
    {
        events.erase(events.begin());
    }

    events.push_back(event);
}

std::vector<Event> EventLogger::search_events(const std::string& type, const std::string& level) const
{
    std::vector<Event> results;

    {
        std::lock_guard<std::mutex> lock(event_mutex);

        for (auto it = events.rbegin(); it != events.rend(); ++it)
        {
            const bool type_matches = type == "*" || it->type == type;
            const bool level_matches = level == "*" || it->level == level;

            if (type_matches && level_matches)
            {
                results.push_back(*it);
            }
        }
    }

    const auto get_priority = [](const std::string& value)
    {
        if (value == "HIGH")   return 3;
        if (value == "MEDIUM") return 2;
        if (value == "LOW")    return 1;
        return 0;
    };

    std::stable_sort
    (
        results.begin(), results.end(),
        [&get_priority](const Event& a, const Event& b)
        {
            return get_priority(a.level) > get_priority(b.level);
        }
    );

    return results;
}