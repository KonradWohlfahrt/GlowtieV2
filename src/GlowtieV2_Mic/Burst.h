#ifndef BURST_h
#define BURST_h

class Burst : public GlowtieEffect
{
  public:
    Burst() : GlowtieEffect(100) { }

    void onStart(int& i) 
    { 
      i = 0; 
      _halfCol = pixels.Color(_red / 2, _green / 2, _blue / 2);
    }
    void onUpdate(int& i) 
    { 
      if (i > 12)
        i = 0;

      if (i < 5) 
      {
        setBar(4 - i, _currentColor);
        setBar(4 + i, _currentColor);
      }
      
      if (5 < i && i < 11)
      {
        setBar(4 - (i - 6), _halfCol);
        setBar(4 + (i - 6), _halfCol);
      }

      if (6 < i && i < 12)
      {
        setBar(4 - (i - 7), 0);
        setBar(4 + (i - 7), 0);
      }

      pixels.show();
      i++;
    }
    void onSound(int& i) { }

    private:
      uint32_t _halfCol;
};

#endif