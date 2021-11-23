/**
 * @file ScanNetworks.cpp
 * @author Carsten Müller (carstenm@mac.com)
 * @brief Network Scanner
 * @version 0.1
 * @date 2021-10-04
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <WiFi.h>
#include <Streaming.h>
#include "ScanNetworks.h"

#include "StatusLED.h"

extern StatusLED LED;
AvailableNetworks_t possibleNetworks;

void scanNetworks(){
    LED.startBlink(CRGB::DarkOrange,STATUS_FREQUENCE);
    // WiFi.scanNetworks will return the number of networks found
    int n = WiFi.scanNetworks();
    if (n == 0) {
        Serial.println("no networks found");
    } else {
        for (int i = 0; i < n; ++i) {
            // Filter by known networks map
            networkMap_t::iterator  it= KnownNetworks.find(WiFi.SSID(i));
            if(it!=KnownNetworks.end()){
                ScannedNetworks_t foundNetwork = ScannedNetworks_t(WiFi.SSID(i),WiFi.RSSI(i));
                possibleNetworks.push(foundNetwork);
            }
        }
    }
    LED.stopBlink();
    Serial << endl;
    if(possibleNetworks.size()){
        LED.setColor(CRGB::DarkGreen/2);
        KnownNetwork_t n = KnownNetworks[possibleNetworks.top().ssid];
        connectTo(n.ssid,n.pwd);
        
    }else{
        Serial << "No known Network found"<<endl;
        LED.setColor(CRGB::DarkRed);
    }
}
void connectTo(KnownNetwork_t n){
    connectTo(n.ssid,n.pwd);
}

void connectTo(String ssid, String pwd){
    LED.startBlink(CRGB::DarkGreen/2,STATUS_FREQUENCE);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid.c_str(), pwd.c_str());
    //Wait for connection
    for(int i=0;i<30 && (WiFi.status() != WL_CONNECTED);i++){
        delay(500);
    }
    LED.stopBlink();
    if(WiFi.status() == WL_CONNECTED){
        LED.setColor(CRGB::DarkGreen);
    }else{
        LED.setColor(CRGB::DarkRed);
    }
}
