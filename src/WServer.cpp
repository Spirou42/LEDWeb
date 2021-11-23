/**
 * @file WServer.cpp
 * @author Carsten Müller (carstenm@mac.com)
 * @brief WebServer, WebSocket handling a.s.o.
 * @version 0.1
 * @date 2021-10-04
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "WServer.h"
#include <AsyncTCP.h>
#include <ESPmDNS.h>
#include <FS.h>
#include <SPIFFS.h>
#include <Streaming.h>
#include <WiFi.h>
#include "Config.h"
#include "StatusLED.h"
#include <ArduinoJson.h>
#include "jsonSerialize.h"
#include "MatrixDemo.h"
#include "WSEvents.h"

/** preliminatry message Type from the client.*/
typedef enum _messageType{
  RGBMessage = 1,
  RequestMessage = 2
} MessageType_t;

AsyncWebServer wServer(80);           // our webserver

AsyncWebSocket ws("/ws");             // websocket instance
AsyncEventSource events("/events");   // eventsoutce

/**
 * @brief converting a CRGB into an int
 * 
 * @param c 
 * @return uint32_t 
 * @todo move to another file
 */
inline uint32_t colorAsInt(CRGB c) { return (c.r << 16) + (c.g << 8) + c.b; }

/**
 * @brief converts a CRGB color to a web hex string
 * 
 * @param c 
 * @return String 
 * @todo move to another file
 */
String colorCode(CRGB c) {
  uint32_t current = colorAsInt(c);
  String p = String(current, 16);
  while (p.length() < 6) {
    p = "0" + p;
  }
  return "#" + p;
}

/**
 * @brief processing a message json color message received from a client
 * 
 * @param color 
 */
void processRGBMessage(JsonObject &color){
  String ColorString = String("New Color: ");
  Serial << "ColorMessage: (R:"<<(int)(color["red"])<<" ,G:"<<(int)(color["green"])<<" ,B:"<<(int)(color["blue"])<<")"<<endl;
  CRGB ledColor = CRGB(color["red"],color["green"],color["blue"]);

  // ColorString += String(ledColor.red);
  // ColorString += ", " + String(ledColor.green);
  // ColorString += ", " + String(ledColor.blue);

  // Serial << ColorString << endl;
  // events.send(ColorString.c_str(), "ota");


  LED.setColor(ledColor);
}

void processPaletteMessage(JsonObject &palette){
  Serial << "PaletteMessage: " << (int)(palette["Palette"]) << endl;
  currentSystemPalette = systemPalettes.begin() + (int)(palette["Palette"]);
}

void processEffectMessage(JsonObject &effect){
  Serial << "EffectMessage: " << (int)(effect["Effect"]) << endl;
  FastLEDAddOns::currentRunningEffect = FastLEDAddOns::systemEffectList.begin() + (int)(effect["Effect"]);
}
void processParameterMessage(JsonObject &parameter){
  String name = parameter["Name"];
  int value = parameter["value"];
  Serial << "ParameterMessage: " << name << " to " << value << endl;
  ValueWrapper *para = wrapperForUIName(name, systemParameterList);
  if(para != NULL){
    para->setValue(value);
  }
}

void processEffectParameterMessage(JsonObject & parameter){
  String name = parameter["Name"];
  int value = parameter["value"];
  int index = parameter["idx"];
  Serial << "EffectParameter: " << name << "("<<index<<") to " << value << endl;
  ValueWrapper *currentWrapper = (*FastLEDAddOns::currentRunningEffect)->parameterAt(index);
  currentWrapper->setValue(value);
}

/**
 * @brief process the WS message from a the client, extract json and dispatch according to type
 * 
 * @param data 
 */
void processClientMessage(char* data){
  StaticJsonDocument<512> jsonDoc;
  DeserializationError error = deserializeJson(jsonDoc, data);
  //JsonObject &root = jsonDoc.parseObject(data);
  if (error){
    Serial << F("Failed parse client message") << endl;
    return;
  }else{
    WSMessageType type = jsonDoc["type"];
    Serial << "Got message of type: "<<type<<endl;
    JsonObject payload = jsonDoc["payload"];
    switch(type){
      case MessageColor:
        processRGBMessage(payload);
        break;
      case MessageEffect:
        processEffectMessage(payload);
        break;
      case MessagePalette:
        processPaletteMessage(payload);
        break;
      case MessageParameter:
        processParameterMessage(payload);
        break;
      case MessageEffectParameter:
        processEffectParameterMessage(payload);
        break;
      default:
        break;
    }
  }
}

/**
 * @brief websocket event handler, handles connections, error and data collection dispatches compleately received messages to the client message processor
 * 
 * @param server 
 * @param client 
 * @param type 
 * @param arg 
 * @param data 
 * @param len 
 */
void onWebSocketEvent(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len){
  if(type == WS_EVT_CONNECT){
    //client connected
    printf("ws[%s][%u] connect\n", server->url(), client->id());
    client->printf("Hello Client %u :)", client->id());
    client->ping();
  } else if(type == WS_EVT_DISCONNECT){
    //client disconnected
    printf("ws[%s][%u] disconnect: %u\n", server->url(), client->id(), client->id());
  } else if(type == WS_EVT_ERROR){
    //error was received from the other end
    printf("ws[%s][%u] error(%u): %s\n", server->url(), client->id(), *((uint16_t*)arg), (char*)data);
  } else if(type == WS_EVT_PONG){
    //pong message was received (in response to a ping request maybe)
    printf("ws[%s][%u] pong[%u]: %s\n", server->url(), client->id(), len, (len)?(char*)data:"");
  } else if(type == WS_EVT_DATA){
    //data packet
    AwsFrameInfo * info = (AwsFrameInfo*)arg;
    if(info->final && info->index == 0 && info->len == len){
      //the whole message is in a single frame and we got all of it's data
      //printf("ws[%s][%u] %s-message[%llu]: ", server->url(), client->id(), (info->opcode == WS_TEXT)?"text":"binary", info->len);
      if(info->opcode == WS_TEXT){
        data[len] = 0;
        //printf("%s\n", (char*)data);
        processClientMessage((char*) data);
      } else {
        for(size_t i=0; i < info->len; i++){
          printf("%02x ", data[i]);
        }
        printf("\n");
      }
      //if(info->opcode == WS_TEXT)
        //client->text("I got your text message");
      //else
        //client->binary("I got your binary message");
    } else {
      //message is comprised of multiple frames or the frame is split into multiple packets
      if(info->index == 0){
        if(info->num == 0)
          printf("ws[%s][%u] %s-message start\n", server->url(), client->id(), (info->message_opcode == WS_TEXT)?"text":"binary");
        printf("ws[%s][%u] frame[%u] start[%llu]\n", server->url(), client->id(), info->num, info->len);
      }

      printf("ws[%s][%u] frame[%u] %s[%llu - %llu]: ", server->url(), client->id(), info->num, (info->message_opcode == WS_TEXT)?"text":"binary", info->index, info->index + len);
      if(info->message_opcode == WS_TEXT){
        data[len] = 0;
        //printf("%s\n", (char*)data);
        processClientMessage((char*) data);
      } else {
        // for(size_t i=0; i < len; i++){
        //   printf("%02x ", data[i]);
        // }
        // printf("\n");
      }

      if((info->index + len) == info->len){
        printf("ws[%s][%u] frame[%u] end[%llu]\n", server->url(), client->id(), info->num, info->len);
        if(info->final){
          printf("ws[%s][%u] %s-message end\n", server->url(), client->id(), (info->message_opcode == WS_TEXT)?"text":"binary");
          // if(info->message_opcode == WS_TEXT)
          //   client->text("I got your text message");
          // else
          //   client->binary("I got your binary message");
        }
      }
    }
  }
}

/**
 * @brief connection handler for the event source, sends current system data to the client after connection
 * 
 * @param client 
 */
void onEventConnection(AsyncEventSourceClient * client){
  Serial << "EventConnection from " << client->client()->remoteIP()<<endl;
  Serial << "Now send Global Effects Palette and Parameter data" << endl;

  /* System Status*/
  sendSystemPalettes(&events);
  sendSystemEffects(&events);
  sendSystemParameters(&events);
  sendEffectParameters(&events);
}

/**
 * @brief template variable processor of the Webserver currently knows about HOSTNAME and CURRENTCOLOR
 * 
 * @param var 
 * @return String 
 */
String processor(const String& var) {
  Serial << var << " replaced with ";
  String result = String("");
  if (var == "HOSTNAME") {
    result = String(HOSTNAME);
  }
  if (var == "CURRENTCOLOR") {
    uint32_t cColor = colorAsInt(LED.color());
    String p = colorCode(LED.color());
    Serial << "nextColor " << _HEX(cColor) << endl;
    Serial << "String: " << p << endl;
    result = p;
  }
  Serial << result << endl;
  return result;
}

/**
 * @brief not found handler
 * 
 * @param request 
 */
void notFound(AsyncWebServerRequest* request) {
  request->send(404, "text/plain", "Not found");
}

/**
 * @brief root handler
 * 
 * @param request 
 */
void handleRoot(AsyncWebServerRequest* request) {
  Serial << "onHandleRoot" << endl;
  if(request->hasParam("nextColor")){
      String newColorValue = "0x"+request->getParam("nextColor")->value();
      // Serial << "Got a new color "<<request->getParam("nextColor")->value() <<endl;
      uint32_t k = strtol(newColorValue.c_str(),0,16);
      // Serial << newColorValue << " ("<<k<<")"<<endl;
      CRGB color = CRGB(k);
      LED.setColor(color);
  }
  request->send(SPIFFS, "/WWW/index.html", String(), false, processor);
}

/**
 * @brief starts webserver activates handles a.s.o.
 * 
 */

void startWEBServer() {
  // wServer.on("/",HTTP_GET, onHandleRoot);
  wServer.onNotFound(notFound);
  //wServer.on("/index.html", HTTP_GET, handleRoot);
  //wServer.rewrite("/static/images/", "/images/");
  wServer.serveStatic("/static", SPIFFS, "/WWW/static/");
  wServer.serveStatic("/images", SPIFFS, "/WWW/images/");
  wServer.serveStatic("/static/images/", SPIFFS, "/WWW/images/");
  wServer.rewrite("/", "/index.html");
  wServer.on("/index.html", HTTP_GET, handleRoot);
  wServer.serveStatic("/", SPIFFS, "/WWW/").setDefaultFile("index.html");
  //.setTemplateProcessor(processor);
  

  ws.onEvent(onWebSocketEvent);
  events.onConnect(onEventConnection);
  wServer.addHandler(&ws);
  wServer.addHandler(&events);


  wServer.begin();
  if (MDNS.begin(HOSTNAME)) {
    MDNS.addService("_http", "_tcp", 80);
    Serial << "MDNS responder started for HTTP on " << WiFi.localIP() << endl;
  }
}

/**
 * @brief shut dowbn the web server, currently does nothing
 * 
 */

void stopWEBServer() {}
