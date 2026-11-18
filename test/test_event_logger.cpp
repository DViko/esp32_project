#include "event_logger.hpp"

#include <iostream>
#include <string>
#include <vector>

int main()
{
    EventLogger logger;

    for (int i = 0; i < 100; ++i)
    {
        Event event;

        event.time = std::to_string(i);
        event.type = "TEMPERATURE";
        event.level = "LOW";

        logger.add_event(event);
    }

    for (int i = 100; i < 105; ++i)
    {
        Event event;

        event.time = std::to_string(i);
        event.type = "TEMPERATURE";
        event.level = "LOW";

        logger.add_event(event);
    }

    const std::vector<Event> results = logger.search_events("*", "*");

    if (results.size() != 100)
    {
        std::cerr << "FAIL: Expected 100 events, got "
                  << results.size() << '\n';

        return 1;
    }

    if (results.back().time != "5")
    {
        std::cerr << "FAIL: Oldest event should be 5, got "
                  << results.back().time << '\n';

        return 1;
    }

    if (results.front().time != "104")
    {
        std::cerr << "FAIL: Newest event should be 104, got "
                  << results.front().time << '\n';

        return 1;
    }

    EventLogger empty_logger;

    const std::vector<Event> empty_results =
        empty_logger.search_events("*", "*");

    if (!empty_results.empty())
    {
        std::cerr << "FAIL: Empty event log should return no results.\n";
        return 1;
    }

    std::cout << "PASS: Empty event log returns no results.\n";

    EventLogger full_logger;

    for (int i = 0; i < 100; ++i)
    {
        Event event;

        event.time = std::to_string(i);
        event.type = "TEMPERATURE";
        event.level = "LOW";

        full_logger.add_event(event);
    }

    const std::vector<Event> full_results =
        full_logger.search_events("*", "*");

    if (full_results.size() != 100)
    {
        std::cerr << "FAIL: Expected 100 events, got "
                << full_results.size() << '\n';

        return 1;
    }

    if (full_results.back().time != "0")
    {
        std::cerr << "FAIL: First event should still be present.\n";
        return 1;
    }

    if (full_results.front().time != "99")
    {
        std::cerr << "FAIL: Last event should be 99.\n";
        return 1;
    }

    std::cout << "PASS: Exactly 100 events are preserved.\n";

    std::cout << "PASS: Event count is limited to 100.\n";
    std::cout << "PASS: Oldest events were removed.\n";
    std::cout << "PASS: Newest events were preserved.\n";

    EventLogger search_logger;

    for (const Event event : {
        Event{"1", "TEMPERATURE", "LOW"},
        Event{"2", "MOTION", "MEDIUM"},
        Event{"3", "LIGHT", "HIGH"},
        Event{"4", "TEMPERATURE", "HIGH"}
    })
    {
        search_logger.add_event(event);
    }

    const std::vector<Event> temperature_results =
        search_logger.search_events("TEMPERATURE", "*");

    if (temperature_results.size() != 2 ||
        temperature_results.front().time != "4" ||
        temperature_results.back().time != "1")
    {
        std::cerr << "FAIL: Temperature filtering returned incorrect results.\n";
        return 1;
    }

    const std::vector<Event> high_results =
        search_logger.search_events("*", "HIGH");

    if (high_results.size() != 2 ||
        high_results.front().time != "4" ||
        high_results.back().time != "3")
    {
        std::cerr << "FAIL: Priority sorting returned incorrect results.\n";
        return 1;
    }

    std::cout << "PASS: Type filtering works.\n";
    std::cout << "PASS: Priority sorting works.\n";
    std::cout << "All EventLogger tests passed!\n";

    return 0;
}