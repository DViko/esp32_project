#include "event_logger.hpp"

#include <iostream>
#include <string>
#include <vector>

/**
 * @brief Reports the result of a test.
 *
 * Prints a PASS or FAIL message and returns the test result.
 *
 * @param test_name Name of the test.
 * @param passed Whether the test passed.
 * @return True if the test passed, otherwise false.
*/
bool report_test(const std::string& test_name, bool passed)
{
    if (passed)
    {
        std::cout << "PASS: " << test_name << '\n';
    }
    else
    {
        std::cerr << "FAIL: " << test_name << '\n';
    }

    return passed;
}

/**
 * @brief Tests FIFO behavior when the event log exceeds its capacity.
 *
 * Adds 105 events and verifies that only the newest 100 events remain.
 *
 * @return True if the FIFO behavior is correct, otherwise false.
*/
bool test_fifo_overflow()
{
    EventLogger logger;

    for (int i = 0; i < 105; ++i)
    {
        Event event;

        event.time = std::to_string(i);
        event.type = "TEMPERATURE";
        event.level = "LOW";

        logger.add_event(event);
    }

    const std::vector<Event> results = logger.search_events("*", "*");

    const bool passed =
        results.size() == 100 &&
        results.front().time == "104" &&
        results.back().time == "5";

    return report_test("FIFO overflow preserves the newest 100 events.", passed);
}

/**
 * @brief Tests searching an empty event log.
 *
 * Verifies that searching an event log without registered events
 * returns an empty result.
 *
 * @return True if the result is empty, otherwise false.
*/
bool test_empty_log()
{
    EventLogger logger;

    const std::vector<Event> results = logger.search_events("*", "*");

    return report_test("Empty event log returns no results.", results.empty());
}

/**
 * @brief Tests the event log at its exact capacity.
 *
 * Adds exactly 100 events and verifies that all events are preserved
 * in the correct order without removing any entries.
 *
 * @return True if all 100 events are preserved, otherwise false.
*/
bool test_exact_capacity()
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

    const std::vector<Event> results = logger.search_events("*", "*");

    const bool passed =
        results.size() == 100 &&
        results.front().time == "99" &&
        results.back().time == "0";

    return report_test("Exactly 100 events are preserved.", passed);
}

/**
 * @brief Tests filtering events by type.
 *
 * Verifies that searching for TEMPERATURE returns only matching
 * events, ordered from newest to oldest.
 *
 * @return True if filtering works correctly, otherwise false.
*/
bool test_type_filtering()
{
    EventLogger logger;

    for (const Event event : {
        Event{"1", "TEMPERATURE", "LOW"},
        Event{"2", "MOTION", "MEDIUM"},
        Event{"3", "LIGHT", "HIGH"},
        Event{"4", "TEMPERATURE", "HIGH"}
    })
    {
        logger.add_event(event);
    }

    const std::vector<Event> results =
        logger.search_events("TEMPERATURE", "*");

    const bool passed =
        results.size() == 2 &&
        results.front().time == "4" &&
        results.back().time == "1";

    return report_test("Event type filtering works correctly.", passed);
}

/**
 * @brief Tests sorting events by severity level.
 *
 * Verifies that events are sorted by priority (HIGH, MEDIUM, LOW)
 * and that events with the same priority are ordered from newest
 * to oldest.
 *
 * @return True if the sorting order is correct, otherwise false.
*/
bool test_priority_sorting()
{
    EventLogger logger;

    for (const Event event : {
        Event{"1", "TEMPERATURE", "LOW"},
        Event{"2", "MOTION", "MEDIUM"},
        Event{"3", "LIGHT", "HIGH"},
        Event{"4", "TEMPERATURE", "HIGH"},
        Event{"5", "MOTION", "LOW"},
        Event{"6", "LIGHT", "MEDIUM"}
    })
    {
        logger.add_event(event);
    }

    const std::vector<Event> results = logger.search_events("*", "*");

    const bool passed =
        results.size() == 6 &&
        results[0].time == "4" &&
        results[1].time == "3" &&
        results[2].time == "6" &&
        results[3].time == "2" &&
        results[4].time == "5" &&
        results[5].time == "1";

    return report_test("Priority sorting and chronological order work correctly.", passed);
}

int main()
{
    bool all_passed = true;

    all_passed &= test_fifo_overflow();
    all_passed &= test_empty_log();
    all_passed &= test_exact_capacity();
    all_passed &= test_type_filtering();
    all_passed &= test_priority_sorting();

    if (!all_passed)
    {
        std::cerr << "\nSome tests failed.\n";
        return 1;
    }

    std::cout << "\nAll EventLogger tests passed!\n";
    return 0;
}