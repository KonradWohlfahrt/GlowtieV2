#ifndef LIBRARYFX_h
#define LIBRARYFX_h

class Breathe : public GlowtieEffect
{
  public:
    Breathe() : GlowtieEffect(15) { }

    void onStart(int& i) { pixels.setMode(15); pixels.start(); }
    void onUpdate(int& i) { pixels.service(); }
    void onSound(int& i) { }
};

class RunningLights : public GlowtieEffect
{
  public:
    RunningLights() : GlowtieEffect() { }

    void onStart(int& i) { pixels.setMode(18); pixels.start(); }
    void onUpdate(int& i) { pixels.service(); }
    void onSound(int& i) { }
};

class Fireworks : public GlowtieEffect
{
  public:
    Fireworks() : GlowtieEffect() { }

    void onStart(int& i) { pixels.setMode(45); pixels.start(); }
    void onUpdate(int& i) { pixels.service(); }
    void onSound(int& i) { pixels.trigger(); }
};

class FireworksRandom : public GlowtieEffect
{
  public:
    FireworksRandom() : GlowtieEffect() { }

    void onStart(int& i) { pixels.setMode(46); pixels.start(); }
    void onUpdate(int& i) { pixels.service(); }
    void onSound(int& i) { pixels.trigger(); }
};

class FireFlicker : public GlowtieEffect
{
  public:
    FireFlicker() : GlowtieEffect() { }

    void onStart(int& i) { pixels.setMode(50); pixels.start(); }
    void onUpdate(int& i) { pixels.service(); }
    void onSound(int& i) { }
};

class TwinkleFOX : public GlowtieEffect
{
  public:
    TwinkleFOX() : GlowtieEffect() { }

    void onStart(int& i) { pixels.setMode(55); pixels.start(); }
    void onUpdate(int& i) { pixels.service(); }
    void onSound(int& i) { }
};

class TwinkleFade : public GlowtieEffect
{
  public:
    TwinkleFade() : GlowtieEffect() { }

    void onStart(int& i) { pixels.setMode(21); pixels.start(); }
    void onUpdate(int& i) { pixels.service(); }
    void onSound(int& i) { }
};

class FlashSparkle : public GlowtieEffect
{
  public:
    FlashSparkle() : GlowtieEffect() { }

    void onStart(int& i) { pixels.setMode(24); pixels.start(); }
    void onUpdate(int& i) { pixels.service(); }
    void onSound(int& i) { }
};

#endif