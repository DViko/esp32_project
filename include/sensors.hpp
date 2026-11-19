#pragma once

#include <DHT.h>

#include <cstdint>
#include <mutex>

/** @brief Stores the latest readings obtained from the connected sensors. */
struct SensorData
{
    /** @brief Temperature reading in degrees Celsius. */
    float temperature = 0.0f;

    /** @brief Relative humidity reading in percent. */
    float humidity = 0.0f;

    /** @brief Raw analog reading from the LDR sensor. */
    int light_level = 0;

    /** @brief Indicates whether motion has been detected by the PIR sensor. */
    bool motion_detected = false;

    /** @brief Indicates whether both temperature and humidity readings are valid. */
    bool dht_valid = false;
};


/** @brief Forward declaration of the event logger class. */
class EventLogger;


/** @brief Provides access to the DHT22 temperature and humidity sensor. */
class DhtSensor
{
    public:
        /**
         * @brief Constructs a DHT22 sensor interface.
         * @param pin GPIO pin connected to the sensor.
        */
        explicit DhtSensor(uint8_t pin);

        /** @brief Initializes the DHT22 sensor. */
        void begin();

        /**
         * @brief Reads the current temperature.
         * @return Temperature in degrees Celsius, or NaN if the reading fails.
        */
        float read_temperature();

        /**
         * @brief Reads the current relative humidity.
         * @return Relative humidity in percent, or NaN if the reading fails.
        */
        float read_humidity();

    private:
        DHT sensor;
};

/** @brief Provides access to the PIR motion sensor. */
class PirSensor
{
    public:
        /**
         * @brief Constructs a PIR sensor interface.
         * @param pin GPIO pin connected to the sensor.
        */
        explicit PirSensor(uint8_t pin);

        /** @brief Configures the PIR sensor pin. */
        void begin();

        /**
         * @brief Checks whether motion is currently detected.
         * @return True if motion is detected, otherwise false.
        */
        bool motion_detected();

    private:
        uint8_t pin;
};


/** @brief Provides access to the LDR light sensor. */
class LdrSensor
{
    public:
        /**
         * @brief Constructs an LDR sensor interface.
         * @param pin GPIO pin connected to the sensor.
        */
        explicit LdrSensor(uint8_t pin);

        /** @brief Configures the LDR sensor pin. */
        void begin();

        /**
         * @brief Reads the raw analog value from the LDR sensor.
         * @return ADC reading from the sensor.
        */
        int read_light_level();

    private:
        uint8_t pin;
};

/** @brief Coordinates initialization and reading of all connected sensors. */
class Sensors
{
    public:
        /** @brief Constructs the sensor manager using the configured GPIO pins. */
        Sensors();

        /** @brief Initializes all connected sensors. */
        void begin();

        /**
         * @brief Reads the current values from all connected sensors.
         * @return A SensorData object containing the latest readings and DHT validity status.
        */
        SensorData read_sensors();

    private:
        DhtSensor dht_sensor;
        PirSensor pir_sensor;
        LdrSensor ldr_sensor;
};

/**
* @brief Continuously monitors sensor readings and registers detected events.
 *
 * @param sensors Reference to the sensor manager.
 * @param logger Reference to the event logger.
 * @param serial_mutex Mutex protecting access to the Serial interface.
*/
void sensor_thread_func(Sensors& sensors, EventLogger& logger, std::mutex& serial_mutex);