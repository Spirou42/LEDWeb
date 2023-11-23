/**
 * @file Config.h
 * @author Carsten Müller (carstenm@mac.com)
 * @brief global configuration file for the System
 * @version 0.1
 * @date 2021-10-04
 *
 * @copyright Copyright (c) 2021
 *
 */

#pragma once
#include <EffectBase.h>

#if USE_CHRISTMASBALL
#define HOSTNAME "espBall"
#elif USE_CYLINDERLAMP
#define HOSTNAME "espTube"
#endif

#define STATUS_FREQUENCE 1.0

/** Definition for the DHT Sensor */
#define DHT_TYPE DHT22
#define DHT_PIN 32

void printLocalTime();