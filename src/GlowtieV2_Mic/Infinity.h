#ifndef INFINITY_h
#define INFINITY_h

const uint8_t _infinityIndex[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 16, 15, 14, 13, 12, 11, 10, 9, 8 };

class Infinity : public GlowtieEffect
{
  public:
    Infinity() : GlowtieEffect(60) { }

    void onStart(int& i) { i = 0; _r = _red; _g = _green; _b = _blue; }
    void onUpdate(int& i) 
    { 
      if (i >= 18)
        i = 0;

      pixels.setPixelColor(_infinityIndex[i], _r, _g, _b);
      pixels.setPixelColor(_infinityIndex[(i + 17) % 18], _r * 2 / 3, _g * 2 / 3, _b * 2 / 3);
      pixels.setPixelColor(_infinityIndex[(i + 16) % 18], _r / 3, _g / 3, _b / 3);
      pixels.setPixelColor(_infinityIndex[(i + 15) % 18], 0, 0, 0);

      pixels.show();
      i++;
    }
    void onSound(int& i) 
    { 
      uint32_t r = getRandomColor();
      _r = r >> 16;
      _g = r >> 8;
      _b = (uint8_t)r;
    }
  
  private:
    uint8_t _r;
    uint8_t _g;
    uint8_t _b;
};

class InfinityFill : public GlowtieEffect
{
  public:
    InfinityFill() : GlowtieEffect(60) { }

    void onStart(int& i) { i = 0; }
    void onUpdate(int& i) 
    { 
      if (i >= 36)
        i = 0;

      if (i < 18)
        pixels.setPixelColor(_infinityIndex[i], _red, _green, _blue);
      else 
      {
        pixels.setPixelColor(_infinityIndex[i - 18], _red / 2, _green / 2, _blue / 2);
        if (i != 18)
          pixels.setPixelColor(_infinityIndex[(i - 1) % 18], 0, 0, 0);
      }

      pixels.show();
      i++;
    }
    void onSound(int& i) { }
};

#endif