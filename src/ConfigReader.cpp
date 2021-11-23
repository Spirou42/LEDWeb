/**
 * @file ConfigReader.cpp
 * @author Carsten Müller (carstenm@mac.com)
 * @brief Wifi Configuration reader
 * @version 0.1
 * @date 2021-10-04
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <Arduino.h>
#include <ArduinoJson.h>
#include <FS.h>
#include <SPIFFS.h>
#include <ConfigReader.h>
#include <Streaming.h>

#define FORMAT_SPIFFS_IF_FAILED true

networkMap_t KnownNetworks;

void listDir(fs::FS &fs, const char * dirname, uint8_t levels){
    Serial.printf("Listing directory: %s\r\n", dirname);

    File root = fs.open(dirname);
    if(!root){
        Serial.println("- failed to open directory");
        return;
    }
    if(!root.isDirectory()){
        Serial.println(" - not a directory");
        return;
    }

    File file = root.openNextFile();
    while(file){
        if(file.isDirectory()){
            Serial.print("  DIR : ");
            Serial.println(file.name());
            if(levels){
                listDir(fs, file.name(), levels -1);
            }
        } else {
            Serial.print("  FILE: ");
            Serial.print(file.name());
            Serial.print("\tSIZE: ");
            Serial.println(file.size());
        }
        file = root.openNextFile();
    }
}

void readFile(fs::FS &fs, const char * path){
    Serial.printf("Reading file: %s\r\n", path);

    File file = fs.open(path);
    if(!file || file.isDirectory()){
        Serial.println("- failed to open file for reading");
        return;
    }

    Serial.println("- read from file:");
    while(file.available()){
        Serial.write(file.read());
    }
    file.close();
}

void readConfig(){
    //listDir(SPIFFS, "/", 0);
    //readFile(SPIFFS,"/Config/KnownNetworks.json");
    // Open file for reading
    File file = SPIFFS.open("/Config/KnownNetworks.json");
    if(!file){
        Serial << "No known networks - file not found"<<endl;
    }
    // Allocate the memory pool on the stack.
    // Don't forget to change the capacity to match your JSON document.
    // Use arduinojson.org/assistant to compute the capacity.
    StaticJsonDocument<512> jsonDoc;

    // Parse the root object
    DeserializationError error = deserializeJson(jsonDoc,file);
    // JsonArray &root = jsonDoc.parseArray(file);
    if (error){
        Serial << F("Failed to read file, using default configuration") << endl;
        return;
    }
    // now extract the network information
    for (int i=0; i<jsonDoc.size(); i++) {
        JsonObject network = jsonDoc[i];
        const char *ssid = network["SSID"];
        const char *pwd = network["pwd"];
        // Serial << "Network: "<<ssid<<endl;
        // Serial << "Passwd:  "<<pwd<<endl<<endl;
        KnownNetwork_t n =  KnownNetwork_t(String(ssid), String(pwd));
        KnownNetworks[String(ssid)] = n;
    }
}

