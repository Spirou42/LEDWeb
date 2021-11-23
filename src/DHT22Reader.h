/**
 * @file DHT22Reader.h
 * @author Carsten Müller (carstenm@mac.com)
 * @brief DHT11 Temperatur and Humidity sensor interface.
 * @version 0.1
 * @date 2021-10-04
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#pragma once
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include "Config.h"

void setupDTH22();
void startDHT22();
void stopDHT22();

sensor_t *getTemperatureSensor();
sensor_t *getHumiditySensor();


extern DHT_Unified dhtSensor;

