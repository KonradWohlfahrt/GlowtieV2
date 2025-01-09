#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <EEPROM.h>

#include "webserver.h"

#define EEPROMSIZE 7

// Battery
#define BATPIN 3
#define BATTERYCHECKINTERVAL 5000
#define LOWBATTERYVOLTS 3500
#define TURNOFFVOLTS 3400


// Buttons
#define B1 9
#define B2 8
#define LONGPRESSTIME 1000

const char *ssid = "GlowtieV2";
const char *password = "password";

const uint8_t _brightness[] = { 10, 20, 30, 40 };
const unsigned long _shuffleTimeMin[] = { 120000, 45000, 30000, 10000 };
const unsigned long _shuffleTimeMax[] = { 360000, 90000, 60000, 20000 };


AsyncWebServer server(80);

/* --- UTILITY START --- */

bool button1() { return digitalRead(B1) == 0; }
bool button2() { return digitalRead(B2) == 0; }
bool anyButton() { return button1() || button2(); }
bool bothButtons() { return button1() && button2(); }

uint16_t getBatteryVoltage() 
{ 
  double avg = 0;
  for (uint8_t i = 0; i < 6; i++)
  {
    avg += analogReadMilliVolts(BATPIN);
    delay(2);
  }
  avg /= 6;
  return (uint16_t)(4.3 * avg);
}
double getBatteryPercent() { return constrain(((getBatteryVoltage() - LOWBATTERYVOLTS) / (4200.0 - LOWBATTERYVOLTS) * 100), 0, 100); }
bool isTimestamp(unsigned long& timestamp, unsigned long interval) 
{
  if (millis() - timestamp >= interval)
  {
    timestamp = millis();
    return true;
  }
  return false;
}

/* --- UTILITY END --- */


/* --- FX START --- */
uint8_t _red = 255;
uint8_t _green = 255;
uint8_t _blue = 255;
uint32_t _currentColor = 0xFFFFFF;

int _effectCounter;

#include "Effect.h"

#include "Static.h"

#include "Burst.h"
#include "Pulse.h"
#include "Bar.h"
#include "Chaser.h"

#include "Infinity.h"
#include "Circles.h"
#include "Symmetry.h"

#include "Rainbow.h"
#include "LibraryFX.h"

const uint8_t _effectsCount = 25;
GlowtieEffect * _effects[] = { new Solid(), new Half(), new RainbowSolid(),
                              new Burst(), new Pulse(), new Bar(), new Scanner(), new Filler(), new Chaser(),
                              new Infinity(), new InfinityFill(),
                              new Circles(), new CirclesFill(),
                              new Symmetry(), new SymmetryFill(),
                              new Rainbow(), new RainbowFill(),
                              new Breathe(), new RunningLights(), new Fireworks(), new FireworksRandom(),
                              new TwinkleFade(), new TwinkleFOX(), new FireFlicker(), new FlashSparkle() };

uint8_t _currentEffectIndex = 0;
uint8_t _currentBrightness;
int _effectSpeed;
bool _effectStatic;
unsigned long __lastFXLoop;
/* --- FX END --- */


// BATTERY
unsigned long __lastBatteryCheck;

// shuffle mode
bool _changeColor;
bool _shuffleActive = false;
uint8_t _shuffleSpeedIndex;
unsigned long _shuffleTime;
unsigned long __lastShuffle;


void setup() 
{
  pinMode(B1, INPUT);
  pinMode(B2, INPUT);
  pinMode(BATPIN, INPUT);
  analogSetAttenuation(ADC_2_5db); // adc range: 0mV - 1050mV
  
  pixels.init();
  pixels.setBrightness(15);
  pixels.setPixelColor(8, RED);
  pixels.show();

  if (!EEPROM.begin(EEPROMSIZE))
  {
    flashAnim(RED, 5);
    lowBattery();
    delay(5000);
    ESP.restart();
  }
  delay(250);
  
  _currentEffectIndex = constrain(EEPROM.read(0), 0, _effectsCount - 1);

  _red = EEPROM.read(1);
  _green = EEPROM.read(2);
  _blue = EEPROM.read(3);
  _currentColor = pixels.Color(_red, _green, _blue);
  pixels.setColor(_currentColor);

  _currentBrightness = constrain(EEPROM.read(4), 0, 3);
  pixels.setBrightness(_brightness[_currentBrightness]);

  _shuffleSpeedIndex = constrain(EEPROM.read(5), 0, 3);
  _changeColor = EEPROM.read(6) == 0;
  

  WiFi.mode(WIFI_AP);
  if (WiFi.softAP(ssid, password, 1, 0, 1))
    scrollAnim(GREEN);
  else
    flashAnim(RED, 5);

  // initialize the server
  server.on("/", handleRoot);
  server.on("/red", HTTP_GET, handleGetRed);
  server.on("/green", HTTP_GET, handleGetGreen);
  server.on("/blue", HTTP_GET, handleGetBlue);
  server.on("/mode", HTTP_GET, handleGetMode);
  server.on("/vcc", HTTP_GET, handleGetVCC);
  server.on("/color", HTTP_GET, handleGetRandomColor);
  server.on("/speed", HTTP_GET, handleGetSpeed);
  server.on("/brightness", HTTP_GET, handleGetBrightness);
  server.begin();

  startEffect();
}
void loop() 
{
  if (isTimestamp(__lastBatteryCheck, BATTERYCHECKINTERVAL)) 
  {
    if (getBatteryVoltage() < LOWBATTERYVOLTS)
    {
      WiFi.mode(WIFI_OFF);
      lowBattery();
      while (true)
      {
        if (getBatteryVoltage() < TURNOFFVOLTS)
        {
          pixels.setPixelColor(8, 20, 0, 0, 0);
          pixels.show();
          esp_deep_sleep_start();
        }
        delay(100);
      }
    }
  }

  handleButtons();
  
  handleEffects();
}

void handleButtons()
{
  // button 1: show battery(short) + save to eeprom(long)
  if (button1())
  {
    bool s = true;
    unsigned long start = millis();
    while (button1())
    {
      handleEffects();
      if (s && millis() - start >= LONGPRESSTIME)
      {
        s = false;
        flashAnim(GREEN, 4);
        EEPROM.write(0, _currentEffectIndex);
        EEPROM.write(1, _red);
        EEPROM.write(2, _green);
        EEPROM.write(3, _blue);
        EEPROM.commit();
        delay(250);
      }
      delay(5);
    }

    if (s)
      showBatteryPercent();
  }
  // button 2: shuffle mode(short)
  else if (button2())
  {
    while (button2())
    {
      handleEffects();
      delay(5);
    }

    _shuffleActive = !_shuffleActive;
    flashSingleAnim(_shuffleActive ? GREEN : RED, 8, 3);
    __lastShuffle = millis();
    _shuffleTime = random(_shuffleTimeMin[_shuffleSpeedIndex], _shuffleTimeMax[_shuffleSpeedIndex]);

    if (!_shuffleActive)
    {
      _currentEffectIndex = constrain(EEPROM.read(0), 0, _effectsCount - 1);
      _red = EEPROM.read(1);
      _green = EEPROM.read(2);
      _blue = EEPROM.read(3);
      updateColor();
    }

    startEffect();
  }
}

void handleEffects()
{
  if (!_effectStatic && isTimestamp(__lastFXLoop, _effectSpeed))
    _effects[_currentEffectIndex]->onUpdate(_effectCounter);

  if (_shuffleActive && isTimestamp(__lastShuffle, _shuffleTime)) 
  {
    if (_changeColor)
    {
      _currentColor = getRandomColor();
      _red = (uint8_t)(_currentColor >> 16);
      _green = (uint8_t)(_currentColor >> 8);
      _blue = (uint8_t)_currentColor; 
    }

    int m;
    do {
      m = random(0, _effectsCount);
    } while (m == _currentEffectIndex);
    _currentEffectIndex = m;
    _shuffleTime = random(_shuffleTimeMin[_shuffleSpeedIndex], _shuffleTimeMax[_shuffleSpeedIndex]);

    startEffect();
  }
}
void startEffect()
{
  pixels.clear();
  pixels.stop();
  pixels.setColor(_currentColor);

  _effectSpeed = _effects[_currentEffectIndex]->getSpeed();
  _effectStatic = _effects[_currentEffectIndex]->getIsStatic();
  _effects[_currentEffectIndex]->onStart(_effectCounter);
}
void updateColor()
{
  _currentColor = pixels.Color(_red, _green, _blue);
  if (_effectStatic)
    startEffect();
}


void handleGetRed(AsyncWebServerRequest* request) { request->send(200, "text/plane", String(_red)); }
void handleGetGreen(AsyncWebServerRequest* request) { request->send(200, "text/plane", String(_green)); }
void handleGetBlue(AsyncWebServerRequest* request) { request->send(200, "text/plane", String(_blue)); }
void handleGetMode(AsyncWebServerRequest* request) { request->send(200, "text/plane", String(_currentEffectIndex)); }
void handleGetVCC(AsyncWebServerRequest* request) { request->send(200, "text/plane", String((int)getBatteryPercent()) + "% (" + String(getBatteryVoltage()) + "mV)"); }
void handleGetRandomColor(AsyncWebServerRequest* request) { request->send(200, "text/plane", (_changeColor ? "1" : "0")); }
void handleGetSpeed(AsyncWebServerRequest* request) { request->send(200, "text/plane", String(_shuffleSpeedIndex)); }
void handleGetBrightness(AsyncWebServerRequest* request) { request->send(200, "text/plane", String(_currentBrightness)); }
void handleRoot(AsyncWebServerRequest* request)
{
  if (request->args() > 0) 
  {
    if (request->hasArg("red"))
    {
      _red = (uint8_t)request->arg("red").toInt();
      _green = (uint8_t)request->arg("green").toInt();
      _blue = (uint8_t)request->arg("blue").toInt();
      updateColor();

      _currentEffectIndex = constrain((uint8_t)request->arg("mode").toInt(), 0, _effectsCount - 1);

      __lastShuffle = millis();

      request->send(200, "text/html", index_html);

      startEffect();
    }
    else
    {
      _currentBrightness = constrain(request->arg("brightness").toInt(), 0, 3);
      EEPROM.write(4, _currentBrightness);
      _shuffleSpeedIndex = constrain(request->arg("speed").toInt(), 0, 3);
      EEPROM.write(5, _shuffleSpeedIndex);
      _changeColor = request->hasArg("randomcolor");
      EEPROM.write(6, _changeColor ? 0 : 255);
      
      

      if (pixels.getBrightness() != _brightness[_currentBrightness])
        pixels.setBrightness(_brightness[_currentBrightness]);

      request->send(200, "text/html", index_html);
  
      flashAnim(GREEN, 4);
      EEPROM.commit();
      delay(250);
      
      startEffect();
    }
  }
  else
    request->send(200, "text/html", index_html);
}