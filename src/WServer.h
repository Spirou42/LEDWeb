/**
 * @file WServer.h
 * @author Carsten Müller (carstenm@mac.com)
 * @brief  public interface to the WEB Server
 * @version 0.1
 * @date 2021-10-04
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once
#include <ESPAsyncWebServer.h>

extern AsyncWebServer wServer;
extern AsyncEventSource events;
void startWEBServer();

typedef enum { 
  MessageColor            = 1, 
  MessagePalette          = 2,
  MessageEffect           = 3,
  MessageParameter        = 4,
  MessageEffectParameter  = 5,
}WSMessageType;