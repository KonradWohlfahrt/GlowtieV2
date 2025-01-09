#ifndef CIRCLES_h
#define CIRCLES_h

const uint8_t _circleLeft[] = { 0, 1, 2, 3, 4, 5, 6, 7 };
const uint8_t _circleRight[] = { 16, 15, 14, 13, 12, 11, 10, 9 };

class Circles : public GlowtieEffect
{
  public:
    Circles() : GlowtieEffect(75) { }

    void onStart(int& i) { i = 0; _r = _red; _g = _green; _b = _blue; }
    void onUpdate(int& i) 
    { 
      if (i >= 8)
        i = 0;

      pixels.setPixelColor(_circleLeft[i], _r, _g, _b);
      pixels.setPixelColor(_circleLeft[(i + 7) % 8], _r / 2, _g / 2, _b / 2);
      pixels.setPixelColor(_circleLeft[(i + 6) % 8], 0, 0, 0);

      pixels.setPixelColor(_circleRight[i], _r, _g, _b);
      pixels.setPixelColor(_circleRight[(i + 7) % 8], _r / 2, _g / 2, _b / 2);
      pixels.setPixelColor(_circleRight[(i + 6) % 8], 0, 0, 0);

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

class CirclesFill : public GlowtieEffect
{
  public:
    CirclesFill() : GlowtieEffect(75) { }

    void onStart(int& i) { i = 0; }
    void onUpdate(int& i) 
    { 
      if (i >= 16)
        i = 0;

      if (i < 8)
      {
        pixels.setPixelColor(_circleLeft[i], _red, _green, _blue);
        pixels.setPixelColor(_circleRight[i], _red, _green, _blue);
      }
      else
      {
        pixels.setPixelColor(_circleLeft[i - 8], _red / 2, _green / 2, _blue / 2);
        pixels.setPixelColor(_circleRight[i - 8], _red / 2, _green / 2, _blue / 2);
        if (i != 8)
        {
          pixels.setPixelColor(_circleLeft[(i - 1) % 8], 0, 0, 0);
          pixels.setPixelColor(_circleRight[(i - 1) % 8], 0, 0, 0);
        }
      }

      pixels.show();
      i++;
    }
    void onSound(int& i) { }
};

#endif