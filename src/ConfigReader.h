/**
 * @file ConfigReader.h
 * @author Carsten Müller (carstenm@mac.com)
 * @brief Wifi Configuration reader interface
 * @version 0.1
 * @date 2021-10-04
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once
#include <FS.h>
#include <map>

void listDir(fs::FS &fs, const char * dirname, uint8_t levels);
void readConfig();

struct KnownNetwork_t{
 public:
  KnownNetwork_t(String ssid=String(), String pwd=String()):ssid(ssid),pwd(pwd){};
  String ssid;
  String pwd;
  bool isConfigured(){
      return (ssid != String()) && (pwd != String());
  };
};

typedef std::map< String,KnownNetwork_t> networkMap_t;

extern networkMap_t KnownNetworks;