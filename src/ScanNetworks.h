/**
 * @file ScanNetworks.h
 * @author Carsten Müller (carstenm@mac.com)
 * @brief Network scanner functions
 * @version 0.1
 * @date 2021-10-04
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#pragma once
#include <Arduino.h>
#include <map>
#include <queue>
#include <vector>
#include "ConfigReader.h"
#include "Config.h"

struct ScannedNetworks_t{
    public:
    ScannedNetworks_t(String ssid=String(),int32_t rssi=0):ssid(ssid),rssi(rssi){};
    String ssid;
    int32_t rssi;
    friend bool operator<(const ScannedNetworks_t& l, const ScannedNetworks_t& r){
        return l.rssi < r.rssi;
    }
};


typedef std::priority_queue<ScannedNetworks_t> AvailableNetworks_t;

template<typename T> void print_queue(T& q) {
    while(!q.empty()) {
        Serial << q.top().ssid<<" (" << q.top().rssi<<")"<<endl;
        q.pop();
    }
    Serial << endl;
}

void scanNetworks();
void connectTo(KnownNetwork_t n);
void connectTo(String ssid, String pwd);