#ifndef AUDIOVISUALIZER_h
#define AUDIOVISUALIZER_h

const int xresGraph = 8;
const int yresGraph = 9;

class FrequencyGraph : public GlowtieEffect
{
  public:
    FrequencyGraph() : GlowtieEffect(50, false, true) { }

    void onStart(int& i) 
    { 
      step = (SAMPLE_BUFFER_SIZE / 2) / xresGraph;
      _halfCol = pixels.Color(_red / 2, _green / 2, _blue / 2);
      i = 0;
    }
    void onUpdate(int& i) 
    {
      performFFT();

      int c = 0;
      for (int i = 0; i < (SAMPLE_BUFFER_SIZE / 2); i += step)
      {
        int v = 0;
        for (int j = 0; j < step; j++)
            v += vReal[i + j];
        sBar[c] = constrain((v / step), 0, 255);
        c++;
      }

      pixels.clear();
      for (int i = 0; i < xresGraph; i++)
      {
        sBar[i] = map(sBar[i], 0, 255, 0, yresGraph);
        
        peaks[i] = peaks[i] - 1;
        if (sBar[i] > peaks[i])
          peaks[i] = sBar[i];
        peaks[i] = sBar[i];

        setFreqBar(i, peaks[i]);
      }
      if (i > 4)
      {
        pixels.setPixelColor(8, 0);
        i = 0;
      }
      pixels.show();

      i++;
    }
    void onSound(int& i) { pixels.setPixelColor(8, _currentColor); pixels.show(); i = 0; }

    private:
      const uint8_t bars[8][2] = {
        { 3, 4 },
        { 2, 5 },
        { 1, 6 },
        { 0, 7 },
        { 16, 9 },
        { 15, 10 },
        { 14, 11 },
        { 13, 12 }
      };

      int step = 0;
      uint8_t sBar[xresGraph];
      uint8_t peaks[xresGraph];
      uint32_t _halfCol;

      void setFreqBar(int i, int y)
      {
        if (y <= 1)
          return;

        if (y <= 3) 
        {
          pixels.setPixelColor(bars[i][0], _halfCol);
        }
        else if (y <= 5)
        {
          pixels.setPixelColor(bars[i][0], _currentColor);
        }
        else if (y <= 7)
        {
          pixels.setPixelColor(bars[i][0], _currentColor);
          pixels.setPixelColor(bars[i][1], _halfCol);
        }
        else
        {
          pixels.setPixelColor(bars[i][0], _currentColor);
          pixels.setPixelColor(bars[i][1], _currentColor);
        }
      }
};

const int xresDots = 17;
const int yresDots = 8;

class FrequencyDots : public GlowtieEffect
{
  public:
    FrequencyDots() : GlowtieEffect(50, false, true) { }

    void onStart(int& i) 
    { 
      step = (SAMPLE_BUFFER_SIZE / 2) / xresDots;
      _halfCol = pixels.Color(_red / 2, _green / 2, _blue / 2);
      i = 0;
    }
    void onUpdate(int& i) 
    {
      performFFT();

      int c = 0;
      for (int i = 0; i < (SAMPLE_BUFFER_SIZE / 2); i += step)
      {
        int v = 0;
        for (int j = 0; j < step; j++)
            v += vReal[i + j];
        sBar[c] = constrain((v / step), 0, 255);
        c++;
      }

      pixels.clear();
      for (int i = 0; i < xresDots; i++)
      {
        sBar[i] = map(sBar[i], 0, 255, 0, yresDots);
        
        peaks[i] = peaks[i] - 1;
        if (sBar[i] > peaks[i])
          peaks[i] = sBar[i];
        peaks[i] = sBar[i];

        if (peaks[i] <= 2)
          pixels.setPixelColor(i, 0);
        else if (peaks[i] <= 5)
          pixels.setPixelColor(i, _halfCol);
        else
          pixels.setPixelColor(i, _currentColor);
      }
      
      pixels.show();
    }
    void onSound(int& i) {  }

    private:
      int step = 0;
      uint8_t sBar[xresDots];
      uint8_t peaks[xresDots];
      uint32_t _halfCol;
};

class VolumeLevel : public GlowtieEffect
{
  public:
    VolumeLevel() : GlowtieEffect(50, false, true) { }

    void onStart(int& i) { }
    void onUpdate(int& i) 
    {
      int height = constrain(getVolumeLevel(LED_COUNT + 1), 0, LED_COUNT + 1);
    
      peak -= 2;
      if (peak < 0)
        peak = 0;
      if (height > peak)
        peak = height;

      for (int i = 0; i < LED_COUNT; i++) {
        if(i >= peak)
          pixels.setPixelColor(i, 0);
        else
          pixels.setPixelColor(i, _currentColor);
      }
      pixels.show();
    }
    void onSound(int& i) { }

    private:
      int peak = 0;
};

#endif