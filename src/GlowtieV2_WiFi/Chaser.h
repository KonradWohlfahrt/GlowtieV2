#ifndef CHASER_h
#define CHASER_h

const uint8_t _chaserIndex[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 8 };

class Chaser : public GlowtieEffect
{
  public:
    Chaser() : GlowtieEffect(75) { }

    void onStart(int& i) 
    { 
      i = 0;
      _col = _currentColor;
      _halfCol = pixels.Color(_red / 2, _green / 2, _blue / 2);
      _thirdCol = pixels.Color(_red / 3, _green / 3, _blue / 3);
    }
    void onUpdate(int& i) 
    { 
      if (i >= 18)
      {
        _col = getRandomColor();
        uint8_t r = _col >> 16;
        uint8_t g = _col >> 8;
        uint8_t b = (uint8_t)_col;

        _halfCol = pixels.Color(r / 2, g / 2, b / 2);
        _thirdCol = pixels.Color(r / 3, g / 3, b / 3);

        i = 0;
      }
        
      pixels.setPixelColor(_chaserIndex[i], _col);
      pixels.setPixelColor(_chaserIndex[(i + 17) % 18], _halfCol);
      pixels.setPixelColor(_chaserIndex[(i + 16) % 18], _thirdCol);
      pixels.setPixelColor(_chaserIndex[(i + 15) % 18], 0);
        
      pixels.show();
      i++;
    }

    private:
      uint32_t _col;
      uint32_t _halfCol;
      uint32_t _thirdCol;
};

#endif