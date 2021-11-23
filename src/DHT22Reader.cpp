/**
 * @file DHT22Reader.cpp
 * @author Carsten Müller (carstenm@mac.com)
 * @brief DHT11 Temperatur and Humidity sensor interface.
 * @version 0.1
 * @date 2021-10-04
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "DHT22Reader.h"
#include <Streaming.h>

DHT_Unified dht(DHT_PIN, DHT_TYPE);
TaskHandle_t DTHTask;
bool DHT_setupCompleted =false;
bool DHT_taskStarted = false;

void DHTReaderTask(void *pvParameter)
{
    sensor_t sensor;
    if(!DHT_setupCompleted){
        setupDTH22();
    }
    dht.temperature().getSensor(&sensor);
    uint32_t minDelay = (sensor.min_delay/1000) ;
    const TickType_t xDelay = minDelay / portTICK_PERIOD_MS;
    sensors_event_t event; 
    while(1){
        printLocalTime();
        dht.temperature().getEvent(&event);
        if (isnan(event.temperature)) {
            Serial << "Error reading temperature!"<<endl;
        } else {
            Serial<<"Temperature: "<<event.temperature<<" °C"<<endl;
        }
        dht.humidity().getEvent(&event);
        if (isnan(event.relative_humidity)) {
            Serial<<"Error reading humidity!"<<endl;
        } else {
            Serial<<"Humidity: "<<event.relative_humidity<<"%"<<endl;
        }
        vTaskDelay(xDelay);
    }
}
void setupDTH22(){
    Serial << "Setup DHT22"<<endl;
    if(!DHT_setupCompleted){
        dht.begin();
        DHT_setupCompleted = true;
    }
}
#define STACKSIZE 2*1024
void startDHT22(){
    if(!DHT_taskStarted){
        xTaskCreate(&DHTReaderTask,"DHT",STACKSIZE,NULL,5,&DTHTask);
        DHT_taskStarted = true;
    }
}

void stopDHT22(){
    if(DHT_taskStarted){
        vTaskSuspend(DTHTask);
        DHT_taskStarted = false;
    }
}

sensor_t tSensor, hSensor;

sensor_t* getTemperatureSensor(){    
    dht.temperature().getSensor(&tSensor);
    return &tSensor;
} 

sensor_t* getHumiditySensor(){
    dht.humidity().getSensor(&hSensor);
    return &hSensor;
}