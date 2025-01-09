#ifndef GLOWTIEBLE_h
#define GLOWTIEBLE_h

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <BLE2901.h>

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define GLOWTIESERVICE_UUID "22a2d43e-5ae6-4b9c-bf45-1d52cd4c4248"
#define BATTERYSERVICE_UUID BLEUUID((uint16_t)0x180F)

const char *bleName = "GlowtieV2"; 


BLEServer *pServer = NULL;

BLEDescriptor BatteryLevelDescriptor(BLEUUID((uint16_t)0x2901));
BLECharacteristic BatteryLevelCharacteristic(BLEUUID((uint16_t)0x2A19), BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY);

// 3x uint8_t: red, green, blue
BLECharacteristic ColorCharacteristic("2a1cb1ea-d364-4d55-9c3b-3334b7813ae3", BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);
// 1x uint8_t: mode index
BLECharacteristic ModeCharacteristic("8de0a20a-4a0c-454d-a570-ed7b278ddc03", BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);
// 3x uint8_t: brightness, shuffle speed, change color
BLECharacteristic PreferencesCharacteristic("9941bd18-84a5-471a-a58d-693c62caa694", BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);


class GlowtieCharacteristicsCallbacks : public BLECharacteristicCallbacks
{
  void onWrite(BLECharacteristic *pCharacteristic)
  {
    size_t l = pCharacteristic->getLength();
    uint8_t *data = pCharacteristic->getData();

    if (pCharacteristic->getUUID().equals(ColorCharacteristic.getUUID()))
    {
      if (l >= 3)
      {
        _red = data[0];
        _green = data[1];
        _blue = data[2];
        updateColor();
        _effects[_currentEffectIndex]->onStart(_effectCounter);
      }
    }
    else if (pCharacteristic->getUUID().equals(ModeCharacteristic.getUUID()))
    {
      if (l >= 1)
      {
        _currentEffectIndex = constrain(data[0], 0, _effectsCount - 1);
        startEffect();
      }
    }
    else
    {
      if (l >= 3) 
      {
        _currentBrightness = constrain(data[0], 0, 3);
        EEPROM.write(4, _currentBrightness);
        if (pixels.getBrightness() != _brightness[_currentBrightness])
          pixels.setBrightness(_brightness[_currentBrightness]);

        _shuffleSpeedIndex = constrain(data[1], 0, 3);
        EEPROM.write(5, _shuffleSpeedIndex);

        _changeColor = data[2] == 0;
        EEPROM.write(6, _changeColor ? 0 : 255);

        flashAnim(GREEN, 4);
        EEPROM.commit();
        delay(250);
        
        startEffect();
      }
    }
  }
};

void setupBLEServer() 
{
  BLEDevice::init(bleName);
  pServer = BLEDevice::createServer();


  // battery level service
  BLEService *pBatService = pServer->createService(BATTERYSERVICE_UUID);
  pBatService->addCharacteristic(&BatteryLevelCharacteristic);
  BatteryLevelDescriptor.setValue("Percentage 0 - 100");
  BatteryLevelCharacteristic.addDescriptor(&BatteryLevelDescriptor);
  BatteryLevelCharacteristic.addDescriptor(new BLE2902());
  pBatService->start();


  // glowtie service
  BLEService *pGlowtieService = pServer->createService(GLOWTIESERVICE_UUID);

  pGlowtieService->addCharacteristic(&ColorCharacteristic);
  ColorCharacteristic.addDescriptor(new BLE2902());
  ColorCharacteristic.setCallbacks(new GlowtieCharacteristicsCallbacks());

  pGlowtieService->addCharacteristic(&ModeCharacteristic);
  ModeCharacteristic.addDescriptor(new BLE2902());
  ModeCharacteristic.setCallbacks(new GlowtieCharacteristicsCallbacks());

  pGlowtieService->addCharacteristic(&PreferencesCharacteristic);
  PreferencesCharacteristic.addDescriptor(new BLE2902());
  PreferencesCharacteristic.setCallbacks(new GlowtieCharacteristicsCallbacks());

  pGlowtieService->start();


  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(BATTERYSERVICE_UUID);
  pAdvertising->addServiceUUID(GLOWTIESERVICE_UUID);
  pAdvertising->setScanResponse(false);
  pAdvertising->setMinPreferred(0x00);  // set value to 0x00 to not advertise this parameter
}

#endif