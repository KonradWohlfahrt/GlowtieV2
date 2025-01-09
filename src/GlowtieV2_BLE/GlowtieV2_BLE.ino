#include <EEPROM.h>

#include "Microphone.h"

#define EEPROMSIZE 7

// Battery
#define BATPIN 3
#define BATTERYCHECKINTERVAL 5000
#define LOWBATTERYVOLTS 3500
#define TURNOFFVOLTS 3400


// Buttons
#define B1 9
#define B2 8
#define DOUBLEPRESSZONE 400
#define LONGPRESSTIME 1000

const uint8_t _brightness[] = { 10, 20, 30, 40 };
const unsigned long _shuffleTimeMin[] = { 120000, 45000, 30000, 10000 };
const unsigned long _shuffleTimeMax[] = { 360000, 90000, 60000, 20000 };


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
bool isTimestamp(unsigned long& timestamp, int interval) 
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
#include "AudioVisualizer.h"

const uint8_t _effectsCount = 28;
GlowtieEffect * _effects[] = { new Solid(), new Half(), new RainbowSolid(),
                              new Burst(), new Pulse(), new Bar(), new Scanner(), new Filler(), new Chaser(),
                              new Infinity(), new InfinityFill(),
                              new Circles(), new CirclesFill(),
                              new Symmetry(), new SymmetryFill(),
                              new Rainbow(), new RainbowFill(),
                              new Breathe(), new RunningLights(), new Fireworks(), new FireworksRandom(),
                              new TwinkleFade(), new TwinkleFOX(), new FireFlicker(), new FlashSparkle(),
                              new FrequencyGraph(), new FrequencyDots(), new VolumeLevel() };

uint8_t _currentEffectIndex = 0;
uint8_t _currentBrightness;
int _effectSpeed;
bool _effectStatic;
bool _effectReactive;
unsigned long _lastFXLoop;
/* --- FX END --- */


// BATTERY
unsigned long _lastBatteryCheck;
uint8_t _currentBatPercent;

// buttons
bool _awaitingDoublePress = false;
unsigned long _lastButtonPress;

// shuffle mode
bool _changeColor;
bool _shuffleActive;
uint8_t _shuffleSpeedIndex;
unsigned long _shuffleTime;
unsigned long _lastShuffle;

// music reactive mode
bool _musicReactActive = true;


#include "BLESetup.h"


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

  setupMicrophone();


  setupBLEServer();

  _currentBatPercent = (uint8_t)getBatteryPercent();
  BatteryLevelCharacteristic.setValue(&_currentBatPercent, 1);

  ColorCharacteristic.setValue(_currentColor);
  ModeCharacteristic.setValue(&_currentEffectIndex, 1);
  uint8_t * pref = new uint8_t[3] { _currentBrightness, _shuffleSpeedIndex, (_changeColor ? 0 : 255) };
  PreferencesCharacteristic.setValue(pref, 3);
  
  scrollAnim(GREEN);


  startEffect();
}
void loop() 
{
  if (isTimestamp(_lastBatteryCheck, BATTERYCHECKINTERVAL)) 
  {
    if (getBatteryVoltage() < LOWBATTERYVOLTS)
    {
      lowBattery();
      BLEDevice::deinit();
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

    _currentBatPercent = (uint8_t)getBatteryPercent();
    BatteryLevelCharacteristic.setValue(&_currentBatPercent, 1);
    BatteryLevelCharacteristic.notify();
  }

  handleButtons();
  
  handleEffects();
}

void handleButtons()
{
  // button 1: show battery(short) + save to eeprom(long)
  if (button1())
  {
    _awaitingDoublePress = false;

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
  // button 2: music reactive mode(long) + start ble advertising(double)
  else if (button2())
  {
    bool s = true;
    unsigned long start = millis();
    while (button2())
    {
      handleEffects();
      if (s && millis() - start >= LONGPRESSTIME)
      {
        s = false;
        _musicReactActive = !_musicReactActive;
        flashAnim(_musicReactActive ? GREEN : RED, 2);
        startEffect();
      }
      delay(5);
    }
    
    if (s)
    {
      if (!_awaitingDoublePress)
      {
        _awaitingDoublePress = true;
        _lastButtonPress = millis();
      }
      else if (millis() - _lastButtonPress <= DOUBLEPRESSZONE)
      {
        _awaitingDoublePress = false;
        
        BLEDevice::startAdvertising();
        flashAnim(GREEN, 2);
        startEffect();
      }
    }
  }

  // button 2 (short press): shuffle mode
  if (_awaitingDoublePress && millis() - _lastButtonPress > DOUBLEPRESSZONE)
  {
    _awaitingDoublePress = false;

    _shuffleActive = !_shuffleActive;
    flashSingleAnim(_shuffleActive ? GREEN : RED, 8, 3);
    _lastShuffle = millis();
    _shuffleTime = random(_shuffleTimeMin[_shuffleSpeedIndex], _shuffleTimeMax[_shuffleSpeedIndex]);

    if (!_shuffleActive)
    {
      _currentEffectIndex = constrain(EEPROM.read(0), 0, _effectsCount - 1);
      _red = EEPROM.read(1);
      _green = EEPROM.read(2);
      _blue = EEPROM.read(3);
      updateColor();

      ColorCharacteristic.setValue(_currentColor);
      ModeCharacteristic.setValue(&_currentEffectIndex, 1);
    }

    startEffect();
  }
}

void handleEffects()
{
  if (_effectReactive || _musicReactActive)
    sampleMicrophone();

  if (!_effectStatic && isTimestamp(_lastFXLoop, _effectSpeed))
    _effects[_currentEffectIndex]->onUpdate(_effectCounter);

  if (_effectReactive || _musicReactActive) 
  {
    sampleVolumeLevel();
    if (getVolumeLevel(255) >= 245)
      _effects[_currentEffectIndex]->onSound(_effectCounter);
  }
    

  if (_shuffleActive && isTimestamp(_lastShuffle, _shuffleTime)) 
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

    ColorCharacteristic.setValue(_currentColor);
    ModeCharacteristic.setValue(&_currentEffectIndex, 1);
  }
}
void startEffect()
{
  pixels.clear();
  pixels.stop();
  pixels.setColor(_currentColor);

  _lastShuffle = millis();

  _effectSpeed = _effects[_currentEffectIndex]->getSpeed();
  _effectStatic = _effects[_currentEffectIndex]->getIsStatic();
  _effectReactive = _effects[_currentEffectIndex]->getIsMusicReactive();
  _effects[_currentEffectIndex]->onStart(_effectCounter);
}
void updateColor()
{
  _currentColor = pixels.Color(_red, _green, _blue);
  pixels.setColor(_currentColor);
  if (_effectStatic)
    startEffect();
}