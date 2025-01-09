#ifndef STATIC_h
#define STATIC_h

class Solid : public GlowtieEffect
{
  public:
    Solid() : GlowtieEffect(STANDARDFXTIME, true) { }

    void onStart(int& i) 
    { 
      pixels.fill(_currentColor, 0, LED_COUNT);
      pixels.show();

      i = 0;
    }
    void onUpdate(int& i) { }
    void onSound(int& i) { }
};

class Half : public GlowtieEffect
{
  public:
    Half() : GlowtieEffect(STANDARDFXTIME, true) { }

    void onStart(int& i) 
    {
      uint32_t c = pixels.Color(_red / 2, _green / 2, _blue / 2);
      pixels.fill(pixels.Color(_red / 4, _green / 4, _blue / 4), 0, LED_COUNT);
      pixels.fill(c, 2, 4);
      pixels.fill(c, 11, 4);
      pixels.setPixelColor(8, 0);
      pixels.show();

      i = 0;
    }
    void onUpdate(int& i) { }
    void onSound(int& i) { }
};

class RainbowSolid : public GlowtieEffect
{
  public:
    RainbowSolid() : GlowtieEffect(STANDARDFXTIME, true) { }

    void onStart(int& i) 
    {
      pixels.rainbow(0);
      pixels.show();
    }
    void onUpdate(int& i)  { }
    void onSound(int& i) { }
};

#endif