#include "sensors.hpp"
#include "config.hpp"

#include <Arduino.h>

DhtSensor::DhtSensor(uint8_t pin) : sensor(pin, DHT22)
{
}

void DhtSensor::begin()
{
    sensor.begin();
}

float DhtSensor::read_temperature()
{
    return sensor.readTemperature();
}

float DhtSensor::read_humidity()
{
    return sensor.readHumidity();
}


PirSensor::PirSensor(uint8_t pin) : pin(pin)
{
}

void PirSensor::begin()
{
    pinMode(pin, INPUT);
}

bool PirSensor::motion_detected()
{
    return digitalRead(pin) == HIGH;
}


LdrSensor::LdrSensor(uint8_t pin) : pin(pin)
{
}

void LdrSensor::begin()
{
    pinMode(pin, INPUT);
}

int LdrSensor::read_light_level()
{
    return analogRead(pin);
}


Sensors::Sensors() : dht_sensor(Config::DHT_PIN), pir_sensor(Config::PIR_PIN), ldr_sensor(Config::LDR_PIN)
{
}

void Sensors::begin()
{
    dht_sensor.begin();
    pir_sensor.begin();
    ldr_sensor.begin();
}

SensorData Sensors::read_sensors()
{
    SensorData data{};

    data.temperature = dht_sensor.read_temperature();
    data.humidity = dht_sensor.read_humidity();
    data.dht_valid = !isnan(data.temperature) && !isnan(data.humidity);
    
    data.light_level = ldr_sensor.read_light_level();
    data.motion_detected = pir_sensor.motion_detected();

    return data;
}