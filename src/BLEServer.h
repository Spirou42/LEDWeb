/**
 * @file BLEServer.h
 * @author Carsten Müller (carstenm@mac.com)
 * @brief BLE Server interfacem for receiving ssid and pwd for a network
 * @version 0.1
 * @date 2021-10-04
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once
#include <Arduino.h>
#include <SimpleBLE.h>

void startBLEServer();
void stopBLEServer();