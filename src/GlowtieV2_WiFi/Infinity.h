#ifndef INFINITY_h
#define INFINITY_h

const uint8_t _infinityIndex[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 16, 15, 14, 13, 12, 11, 10, 9, 8 };

class Infinity : public GlowtieEffect
{
  public:
    Infinity() : GlowtieEffect(60) { }

    void onStart(int& i) { i = 0; }
    void onUpdate(int& i) 
    { 
      if (i >= 18)
        i = 0;

      pixels.setPixelColor(_infinityIndex[i], _red, _green, _blue);
      pixels.setPixelColor(_infinityIndex[(i + 17) % 18], _red * 2 / 3, _green * 2 / 3, _blue * 2 / 3);
      pixels.setPixelColor(_infinityIndex[(i + 16) % 18], _red / 3, _green / 3, _blue / 3);
      pixels.setPixelColor(_infinityIndex[(i + 15) % 18], 0, 0, 0);

      pixels.show();
      i++;
    }
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
};

#endif