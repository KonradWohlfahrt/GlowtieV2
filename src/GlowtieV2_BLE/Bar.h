#ifndef BAR_h
#define BAR_h

class Bar : public GlowtieEffect
{
  public:
    Bar() : GlowtieEffect(100) { }

    void onStart(int& i) 
    { 
      i = 0;
      _halfCol = pixels.Color(_red / 2, _green / 2, _blue / 2);
      _fromLeft = true;
    }
    void onUpdate(int& i) 
    { 
      if (i > 18)
      {
        i = 0;
        _fromLeft = !_fromLeft;
      }
        

      if (i < 9)
      {
        if (_fromLeft)
          setBar(i, _currentColor);
        else
          setBar(8 - i, _currentColor);
      }

      
      if (8 < i && i < 18)
      {
        if (_fromLeft)
          setBar(i - 9, _halfCol);
        else
          setBar(8 - (i - 9), _halfCol);
      }

      
      if (9 < i)
      {
        if (_fromLeft)
          setBar(i - 10, 0);
        else
          setBar(8 - (i - 10), 0);
      }
        
      pixels.show();
      i++;
    }
    void onSound(int& i) { }

    private:
      uint32_t _halfCol;
      bool _fromLeft;
};

const uint8_t _scannerMoveIndex[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 7, 6, 5, 4, 3, 2, 1 };
class Scanner : public GlowtieEffect
{
  public:
    Scanner() : GlowtieEffect(125) { }

    void onStart(int& i) 
    { 
      i = 0;
      _curCol = pixels.Color(_red, _green, _blue);
      _halfCol = pixels.Color(_red / 2, _green / 2, _blue / 2);
      _thirdCol = pixels.Color(_red / 3, _green / 3, _blue / 3);
    }
    void onUpdate(int& i) 
    { 
      if (i >= 16)
        i = 0;

      setBar(_scannerMoveIndex[i], _curCol);
      setBar(_scannerMoveIndex[(i + 15) % 16], _halfCol);
      setBar(_scannerMoveIndex[(i + 14) % 16], _thirdCol);
      setBar(_scannerMoveIndex[(i + 13) % 16], 0);
        
      pixels.show();
      i++;
    }
    void onSound(int& i) 
    { 
      _curCol = getRandomColor();
      _halfCol = pixels.Color((uint8_t)(_curCol >> 16) / 2, (uint8_t)(_curCol >> 8) / 2, ((uint8_t)_curCol) / 2);
      _thirdCol = pixels.Color((uint8_t)(_curCol >> 16) / 3, (uint8_t)(_curCol >> 8) / 3, ((uint8_t)_curCol) / 3);
    }

    private:
      uint32_t _curCol;
      uint32_t _halfCol;
      uint32_t _thirdCol;
};

class Filler : public GlowtieEffect
{
  public:
    Filler() : GlowtieEffect(75) { }

    void onStart(int& i) 
    { 
      i = 0;
      _randomPhase = false;
      _c = _currentColor;
      _v = varyColor(_c);
    }
    void onUpdate(int& i) 
    { 
      if (i > 19)
      {
        i = 0;
        _randomPhase = !_randomPhase;
        if (_randomPhase)
          _c = getRandomColor();
        else
          _c = _currentColor;
        _v = varyColor(_c);
      }
      
      if (i < 9)
        setBar(i, _c);
      else if (9 < i && i < 19)
        setBar(8 - (i - 10), _v);
        
      pixels.show();
      i++;
    }
    void onSound(int& i) { }

    private:
      uint32_t _c;
      uint32_t _v;
      bool _randomPhase;
};

#endif