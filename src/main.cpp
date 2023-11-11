#include <Arduino.h>
#include <Streaming.h>
#include <WiFi.h>
#include <FS.h>
#include <SPIFFS.h>
#include <time.h>
#include "ConfigReader.h"
#include "ScanNetworks.h"
#include "WServer.h"
#include "BLEServer.h"
#include "Config.h"
#include "FastLEDAddOns.h"
#include "avrQueue.h"


#include <LEDEffects.h>
#include <WSEvents.h>

#define FORMAT_SPIFFS_IF_FAILED true

const char* ntpServer = "de.pool.ntp.org";
const long  gmtOffset_sec = 3600;
const int   daylightOffset_sec = 3600;

/** Simple arduino Task Scheduler */
avrQueue taskQueue;

void printLocalTime()
{
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%d/%m/%Y %H:%M:%S");
}

bool SetupWiFi()
{
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();  
  delay(100);
  return true;
}

bool SetupFileSystem()
{
  if(!SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED)){
    Serial.println("SPIFFS Mount Failed");
    return false;
  }
  return true;
}

void dumpParameters(Effect* currentEffect)
{
  Serial<<"Name: "<<currentEffect->name()<<endl;
  for(size_t k = 0;k<currentEffect->numberOfParameters();k++){
    Serial<<"Parameter: "<<currentEffect->parameterNameAt(k)<<"\t";
    ValueWrapper* vw = currentEffect->parameterAt(k);
    if(vw){
        Serial << vw->getValue()<<"\t("<<vw->getMinValue()<<", "<<vw->getMaxValue()<<")"<<endl;
    }else{
      Serial << endl;
    }
  }
  Serial << "----"<<endl;
}


void postFrameCallback(unsigned long now){
  static int hueDelay = 0;
  static EffectList::iterator lastEffect;
  static PaletteList::iterator lastPalette;
  static int lastBrightness = 0;
  
  int16_t hFD = /*hueStepWrapper.frameDelay() + */ globalHueFrameDelay;
  int16_t hueStep =  /*hueStepWrapper.*/ globalHueStep/*()*/;
  
  if (hueDelay >= hFD) {
    globalHue+=hueStep;
    hueDelay = 0;
  } else {
    hueDelay ++;
  }
  if(lastEffect != FastLEDAddOns::currentRunningEffect){
    Serial <<"Switch detected"<<endl;
    Effect *p = (*FastLEDAddOns::currentRunningEffect);
    currentFrameRate = FastLEDAddOns::requestedFrameRate;
    // update Parameters in UI here
    if(p->numberOfParameters()){
      // if(!effectParameterLabel->getSuperview()){
      //   SystemMenu.addSubview(effectParameterLabel);
      //   SystemMenu.sizeToFit();
      //   SystemMenu.layoutList();
      // }
      // setupParameters();
    }else{
      // effectParameterLabel->removeFromSuperview();
      // SystemMenu.sizeToFit();
      // tft.fillScreen(0);
      // SystemMenu.layoutList();
      // TFT_UI::ResponderStack::iterator iter = TFT_UI::responderStack.begin();
      // while(iter !=TFT_UI::responderStack.end() ){
      //   Serial << "Redraw"<<_HEX((unsigned long)*iter)<<endl;
      //   (*iter)->setNeedsRedraw();
      //   (*iter)->redraw();
      //   iter++;
      //}
    }
    lastEffect = FastLEDAddOns::currentRunningEffect;
    dumpParameters(*FastLEDAddOns::currentRunningEffect);
    sendSystemEffects(&events);
    sendSystemParameters(&events);
    sendEffectParameters(&events);
  }

  if(lastPalette != currentSystemPalette){
    Serial << "Palette Switch detected" << endl;
    Serial << "Palette " <<  (currentSystemPalette - systemPalettes.begin()) << endl;
    lastPalette = currentSystemPalette;
    sendSystemPalettes(&events);
  }
  if(lastBrightness != ledBrightness){
    lastBrightness = ledBrightness;
    FastLED.setBrightness(ledBrightness);
  }
}



static bool serviceStarted = false;

void startServices(){
    startWEBServer();
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    printLocalTime();
    //startDHT22();
    serviceStarted = true;
    startupEffects(&taskQueue,postFrameCallback);
}

void setup() 
{
  Serial.begin(115200);
  initialiseLEDs();
  startupBackbuffer(&taskQueue);
  LED.attachPins(27, 32, 33);
  LED.startBlink(CRGB::DarkRed,STATUS_FREQUENCE);

  // put your setup code here, to run once:
  
  delay(200);
  
  if( !SetupWiFi() ){
    Serial << "WiFi init failed"<< endl;
  }
  if( !SetupFileSystem() ){
    Serial << "Filesystem mount failed"<< endl;
  }
  LED.stopBlink();
  LED.setColor(CRGB::Orange);
  
  readConfig();
  delay(100);
  LED.setColor(CRGB::DeepPink);
  scanNetworks();
  if(WiFi.isConnected()){
    Serial << "Connected as "<<HOSTNAME<<".local ("<<WiFi.localIP()<<") to "<<WiFi.SSID()<<endl;
    startServices();
  }else{
    Serial << "Connection to "<<WiFi.SSID()<<" failed"<<endl;
    //startBLEServer();
  }

}


void loop() 
{
  static int lastFrameRate = 0;

  if(WiFi.isConnected() && !serviceStarted){
    Serial << "Connected as "<<HOSTNAME<<".local ("<<WiFi.localIP()<<") to "<<WiFi.SSID()<<endl;
    startServices();
  }
  taskQueue.Run(millis());

  // if(currentFrameRate != FastLEDAddOns::requestedFrameRate){
  //   lastFrameRate = currentFrameRate = FastLEDAddOns::requestedFrameRate;
  //   adjustFrameRate(&taskQueue, currentFrameRate);
  // }

  if(lastFrameRate != currentFrameRate){
    lastFrameRate = currentFrameRate;
    adjustFrameRate(&taskQueue, currentFrameRate);
  }
  // if (WiFi.isConnected()) {
  //   EVERY_N_MILLISECONDS( 2) {
  //     pacifica_loop();
  //     FastLED.show();
  //   }
  // }
  // if(WiFi.isConnected()){
  //   EVERY_N_SECONDS(120) { FastLEDAddOns::currentRunningEffect++; }
  // }

  EVERY_N_MINUTES(10) {
    FastLEDAddOns::currentRunningEffect++;
    if (FastLEDAddOns::currentRunningEffect == FastLEDAddOns::systemEffectList.end()) {
      FastLEDAddOns::currentRunningEffect = FastLEDAddOns::systemEffectList.begin();
    }
  }
  EVERY_N_MINUTES(3) { 
    currentSystemPalette++;
    if(currentSystemPalette == systemPalettes.end()){
      currentSystemPalette = systemPalettes.begin();
    }
  }
}