#ifndef EFFECT_h
#define EFFECT_h

#include <WS2812FX.h>

#define LED_PIN 18
#define LED_COUNT 17
#define STANDARDFXTIME 50

WS2812FX pixels = WS2812FX(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);



uint32_t getRandomColor() { return pixels.ColorHSV(random(65536)); }
uint8_t varyColor(uint8_t c)
{
  uint8_t n = (uint8_t)(c / (random(10, 30) / 10.0f));
  return n == 0 ? c : n;
}
uint32_t varyColor(uint32_t color) { return pixels.Color(varyColor((uint8_t)(color >> 16)), varyColor((uint8_t)(color >> 8)), varyColor((uint8_t)color)); }


#define BARCOUNT 9
const uint8_t _barIndex[] = { 3, 2, 1, 0, 8, 16, 15, 14, 13, 
                              4, 5, 6, 7, 8, 9,  10, 11, 12 };
void setBar(uint8_t index, uint32_t color)
{
  if (index < 0 || index > 8)
    return;
  pixels.setPixelColor(_barIndex[index], color);
  pixels.setPixelColor(_barIndex[index + 9], color);
  pixels.show();
}


void disableLEDs()
{
  pixels.clear();
  pixels.show();
}
void lowBattery() 
{
  pixels.clear();
  pixels.setPixelColor(8, 50, 0, 0);
  pixels.show();
}


void scrollAnim(uint32_t color)
{
  for (int i = 0; i < LED_COUNT; i++)
  {
    pixels.setPixelColor(i, color);
    pixels.show();
    delay(STANDARDFXTIME);
  }
  for (int i = 0; i < LED_COUNT; i++)
  {
    pixels.setPixelColor(i, 0);
    pixels.show();
    delay(STANDARDFXTIME);
  }
}
void scrollBarAnim(uint32_t color)
{
  for (int i = 0; i < BARCOUNT; i++)
  {
    setBar(i, color);
    delay(STANDARDFXTIME);
  }
  for (int i = 0; i < BARCOUNT; i++)
  {
    setBar(i, 0);
    delay(STANDARDFXTIME);
  }
}

void flashAnim(uint32_t color, int amount)
{
  for (int i = 0; i < amount * 2; i++)
  {
    if (i % 2 == 0)
      pixels.fill(color, 0, LED_COUNT);
    else
      pixels.clear();
    
    pixels.show();
    delay(STANDARDFXTIME);
  }
}
void flashSingleAnim(uint32_t color, int led, int amount)
{
  for (int i = 0; i < amount * 2; i++)
  {
    if (i % 2 == 0)
      pixels.setPixelColor(led, color);
    else
      pixels.setPixelColor(led, 0);
    
    pixels.show();
    delay(STANDARDFXTIME);
  }
}


void showBatteryPercent()
{
  for (int i = 0; i < 9; i++)
  {
    setBar(i, GREEN);
    delay(STANDARDFXTIME);
  }

  int p = (int)getBatteryPercent();
  int ledAmount = (int)(0.08 * p + 0.5);
  uint32_t c = GREEN;
  if (p <= 20)
    c = RED;
  else if (p <= 50)
    c = ORANGE;

  for (int i = 8; i >= 0; i--)
  {
    if (i > ledAmount)
      setBar(i, 0);
    else
      setBar(i, c);
    delay(STANDARDFXTIME);
  }

  delay(2000);
}


class GlowtieEffect 
{
    public:
        GlowtieEffect(int effectSpeed = STANDARDFXTIME, bool isStatic = false, bool musicReactive = false) : _effectSpeed(effectSpeed), _isStatic(isStatic), _isMusicReactive(musicReactive) {}
        virtual void onStart(int& i) = 0;
        virtual void onUpdate(int& i) = 0;
        virtual void onSound(int& i) = 0;
        int getSpeed() { return _effectSpeed; }
        bool getIsStatic() { return _isStatic; }
        bool getIsMusicReactive() { return _isMusicReactive; }

    private:
        int _effectSpeed;
        bool _isStatic;
        bool _isMusicReactive;
};

#endif