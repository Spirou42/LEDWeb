/**
 * @file BLEServer.cpp
 * @author Carsten Müller (carstenm@mac.com)
 * @brief BLE Server for receiving ssid and pwd for a network
 * @version 0.1
 * @date 2021-10-04
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <Streaming.h>
#include "esp_bt_main.h"
#include "BLEServer.h"
#include "WServer.h"
#include "ScanNetworks.h"
#include "ConfigReader.h"
// https://www.uuidgenerator.net/

#define SERVICE_UUID "c106088c-807d-416f-907a-b42a0adae5e4"
#define CHARACTERISTIC_SSID_UUID "9032df8b-2efd-4b00-8bf0-11bed25c370b"
#define CHARACTERISTIC_PWD_UUID "d9e79c7f-8bbb-4166-b989-b2821f4b6389"

KnownNetwork_t BLENetwork;
BLEServer *pServer;
BLEService *pService;
BLEAdvertising *pAdvertising;

class MyCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
        String value = String(pCharacteristic->getValue().c_str());
        if (value.length() > 0) {
            Serial << "*********"<<endl;
            Serial <<"New value: "<<value<<endl;
            Serial << "*********"<<endl;
        }
        if(String(pCharacteristic->getUUID().toString().c_str()) == String(CHARACTERISTIC_SSID_UUID)){
            Serial << "SSID changed"<<endl;
            BLENetwork.ssid = String(pCharacteristic->getValue().c_str());
        }else if(String(pCharacteristic->getUUID().toString().c_str()) == String(CHARACTERISTIC_PWD_UUID)){
            Serial << "PWD changed"<<endl;
            BLENetwork.pwd = String(pCharacteristic->getValue().c_str());
        }
        if (BLENetwork.isConfigured()){
            Serial << "Ready "<<BLENetwork.ssid<<", "<<BLENetwork.pwd<<endl;
            stopBLEServer();
            connectTo(BLENetwork);
        }else{
            Serial << "incomplet"<<endl;
        }
    }
};

void startBLEServer(){
    static MyCallbacks *callbacks = new MyCallbacks(); 
    Serial << "BLE Server starting"<<endl;
    BLEDevice::init("MyESP32");
    pServer = BLEDevice::createServer();
    pService = pServer->createService(SERVICE_UUID);
    BLECharacteristic *ssidCharacteristic = pService->createCharacteristic(
        CHARACTERISTIC_SSID_UUID,
        BLECharacteristic::PROPERTY_READ |
        BLECharacteristic::PROPERTY_WRITE
        );
    ssidCharacteristic->setCallbacks(callbacks);
    ssidCharacteristic->setValue("SSID");
    BLECharacteristic *pwdCharacteristic = pService->createCharacteristic(
        CHARACTERISTIC_PWD_UUID,
        BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE
    );
    pwdCharacteristic->setCallbacks(callbacks);
    pwdCharacteristic->setValue("pwd");
    pService->start();
    pAdvertising = pServer->getAdvertising();
    pAdvertising->start();
}

void stopBLEServer(){
    Serial << "BLE stopping"<<endl;
     btStop();
    esp_bt_controller_disable();
    esp_bt_controller_deinit();
    Serial << "bt Controller deinitialized"<<endl;
    //esp_bluedroid_disable();
    //esp_bluedroid_deinit();
    Serial << "bluedroid deinitialized"<<endl;
    
    // //pAdvertising->stop();
    // Serial << "Delete Service"<<endl;
    // pService->stop();
    // //pService->executeDelete();
    // Serial << "Remove Service"<<endl;
    // pServer->removeService(pService);
    // Serial << "Stop Service"<<endl;
    // pService->stop();
    // // delete(pAdvertising);
    // // delete(pService);
    // // delete(pServer);
    Serial << "BLE stopped"<<endl;
}