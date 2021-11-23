/**
 * @file WSEvents.h
 * @author Carsten Müller (carstenm@mac.com)
 * @brief Some Websocket Events sender function
 * @version 0.1
 * @date 2021-10-13
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once

#include "jsonSerialize.h"
#include "WServer.h"

void sendSystemPalettes(AsyncEventSource *events);
void sendSystemEffects(AsyncEventSource *events);
void sendSystemParameters(AsyncEventSource *events);
void sendEffectParameters(AsyncEventSource *events);