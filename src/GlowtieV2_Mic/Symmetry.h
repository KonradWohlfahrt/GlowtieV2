#ifndef SYMMETRY_h
#define SYMMETRY_h

const uint8_t _symmetryLeft[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8 };
const uint8_t _symmetryRight[] = { 9, 10, 11, 12, 13, 14, 15, 16, 8 };

class Symmetry : public GlowtieEffect
{
  public:
    Symmetry() : GlowtieEffect(75) { }

    void onStart(int& i) { i = 0; _r = _red; _g = _green; _b = _blue; }
    void onUpdate(int& i) 
    { 
      if (i >= 9)
        i = 0;

      pixels.setPixelColor(_symmetryLeft[i], _r, _g, _b);
      pixels.setPixelColor(_symmetryLeft[(i + 8) % 9], _r / 2, _g / 2, _b / 2);
      pixels.setPixelColor(_symmetryLeft[(i + 7) % 9], _r / 3, _g / 3, _b / 3);
      pixels.setPixelColor(_symmetryLeft[(i + 6) % 9], 0, 0, 0);

      pixels.setPixelColor(_symmetryRight[i], _r, _g, _b);
      pixels.setPixelColor(_symmetryRight[(i + 8) % 9], _r / 2, _g / 2, _b / 2);
      pixels.setPixelColor(_symmetryRight[(i + 7) % 9], _r / 3, _g / 3, _b / 3);
      pixels.setPixelColor(_symmetryRight[(i + 6) % 9], 0, 0, 0);

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

class SymmetryFill : public GlowtieEffect
{
  public:
    SymmetryFill() : GlowtieEffect(75) { }

    void onStart(int& i ) { i = 0; }
    void onUpdate(int& i) 
    { 
      if (i >= 18)
        i = 0;

      if (i < 9)
      {
        pixels.setPixelColor(_symmetryLeft[i], _red, _green, _blue);
        pixels.setPixelColor(_symmetryRight[i], _red, _green, _blue);
      }
      else
      {
        pixels.setPixelColor(_symmetryLeft[i - 9], _red / 2, _green / 2, _blue / 2);
        pixels.setPixelColor(_symmetryRight[i - 9], _red / 2, _green / 2, _blue / 2);
        if (i != 9)
        {
          pixels.setPixelColor(_symmetryLeft[(i - 1) % 9], 0, 0, 0);
          pixels.setPixelColor(_symmetryRight[(i - 1) % 9], 0, 0, 0);
        }
      }

      pixels.show();
      i++;
    }
    void onSound(int& i) { }
};

#endif