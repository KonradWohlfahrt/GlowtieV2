#ifndef PULSE_h
#define PULSE_h

const uint8_t _pulseLeft[] = { 4, 3, 2, 1, 0, 1, 2, 3, 4 };
const uint8_t _pulseRight[] = { 4, 5, 6, 7, 8, 7, 6, 5, 4 };

class Pulse : public GlowtieEffect
{
  public:
    Pulse() : GlowtieEffect(125) { }

    void onStart(int& i) 
    { 
      i = 0;
      _curCol = pixels.Color(_red, _green, _blue);
      _halfCol = pixels.Color(_red / 2, _green / 2, _blue / 2);
    }
    void onUpdate(int& i) 
    {
      if (i > 12)
        i = 0;

      if (i == 12)
        pixels.clear();

      if (0 < i && i < 10)
      {
        setBar(_pulseLeft[i - 1], _curCol);
        setBar(_pulseRight[i - 1], _curCol);
      }
      if (1 < i && i < 11)
      {
        setBar(_pulseLeft[(i + 7) % 9], _halfCol);
        setBar(_pulseRight[(i + 7) % 9], _halfCol);
      }
      if (2 < i && i < 12)
      {
        setBar(_pulseLeft[(i + 6) % 9], 0);
        setBar(_pulseRight[(i + 6) % 9], 0);
      }

      pixels.show();
      i++;
    }
    void onSound(int& i) 
    { 
      _curCol = getRandomColor();
      _halfCol = pixels.Color((uint8_t)(_curCol >> 16) / 2, (uint8_t)(_curCol >> 8) / 2, ((uint8_t)_curCol) / 2);
    }

    private:
      uint32_t _halfCol;
      uint32_t _curCol;
};

#endif