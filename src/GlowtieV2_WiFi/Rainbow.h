#ifndef RAINBOW_h
#define RAINBOW_h

class Rainbow : public GlowtieEffect
{
  public:
    Rainbow() : GlowtieEffect(20) { }

    void onStart(int& i) { }
    void onUpdate(int& i) 
    {
      if (_firstPixelHue >= 5*65536)
        _firstPixelHue = 0;
      pixels.rainbow(_firstPixelHue);
      pixels.show();
      _firstPixelHue += 256;
    }

    private:
      long _firstPixelHue = 0;
};

class RainbowFill : public GlowtieEffect
{
  public:
    RainbowFill() : GlowtieEffect(20) { }

    void onStart(int& i) { }
    void onUpdate(int& i) 
    {
      pixels.fill(pixels.ColorHSV(_hue), 0, LED_COUNT);
      pixels.show();
      _hue += 256;
    }

    private:
      int _hue = 0;
};

#endif