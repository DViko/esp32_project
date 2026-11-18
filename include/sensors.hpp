#pragma once

#include <DHT.h>

#include <cstdint>
#include <mutex>

struct SensorData
{
    float temperature = 0.0f;
    float humidity = 0.0f;
    int light_level = 0;
    bool motion_detected = false;
    bool dht_valid = false;
};


class EventLogger;


class DhtSensor
{
    public:
        explicit DhtSensor(uint8_t pin);

        void begin();
        float read_temperature();
        float read_humidity();

    private:
        DHT sensor;
};


class PirSensor
{
    public:
        explicit PirSensor(uint8_t pin);

        void begin();
        bool motion_detected();

    private:
        uint8_t pin;
};


class LdrSensor
{
    public:
        explicit LdrSensor(uint8_t pin);

        void begin();
        int read_light_level();

    private:
        uint8_t pin;
};


class Sensors
{
    public:
        Sensors();

        void begin();

        SensorData read_sensors();

    private:
        DhtSensor dht_sensor;
        PirSensor pir_sensor;
        LdrSensor ldr_sensor;
};

void sensorThreadFunc(Sensors& sensors, EventLogger& logger, std::mutex& serial_mutex);