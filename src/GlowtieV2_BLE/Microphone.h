#ifndef MICROPHONE_h
#define MICROPHONE_h

// inmp441 reference code: https://github.com/slgrobotics/Misc/blob/master/Arduino/Sketchbook/ESP32_INMP441_Setup/ESP32_INMP441_Setup.ino
// volume level reference code: https://learn.adafruit.com/trinket-sound-reactive-led-color-organ/code

#include <arduinoFFT.h>
#include <driver/i2s.h>

#define I2S_WS 6
#define I2S_SD 4
#define I2S_SCK 5
#define I2S_PORT I2S_NUM_0
#define I2S_MIC_CHANNEL I2S_CHANNEL_FMT_ONLY_LEFT

#define MIC_NOISE 100

#define SAMPLE_BUFFER_SIZE 64
#define SAMPLE_RATE 44100

#define PEAKTOPEAK_BUFFER_SIZE 32
#define MIN_PEAKTOPEAK_DISTANCE 25


// --------------
// I2S MICROPHONE
// --------------

const i2s_config_t i2s_config = {
    .mode = i2s_mode_t(I2S_MODE_MASTER | I2S_MODE_RX),
    .sample_rate = SAMPLE_RATE,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
    //.bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT,
    .channel_format = I2S_MIC_CHANNEL,
    .communication_format = i2s_comm_format_t(I2S_COMM_FORMAT_STAND_I2S),
    .intr_alloc_flags = 0,
    //.intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = 4,
    .dma_buf_len = 1024,
    .use_apll = false,
    .tx_desc_auto_clear = false,
    .fixed_mclk = 0 };
const i2s_pin_config_t i2s_pin_config = {
    .bck_io_num = I2S_SCK,
    .ws_io_num = I2S_WS,
    .data_out_num = I2S_PIN_NO_CHANGE,
    .data_in_num = I2S_SD };

int16_t sBuffer[SAMPLE_BUFFER_SIZE];
//int32_t sBuffer[SAMPLE_BUFFER_SIZE];
int sampleCount = 0;

void setupMicrophone() 
{
  delay(1000);
  i2s_driver_install(I2S_PORT, &i2s_config, 0, NULL);
  i2s_set_pin(I2S_PORT, &i2s_pin_config);
  i2s_start(I2S_PORT);
  delay(500);
}
bool sampleMicrophone()
{
  for (int i = 0; i < SAMPLE_BUFFER_SIZE; i++)
    sBuffer[i] = 0;

  size_t bytesIn = 0;
  esp_err_t result = i2s_read(I2S_PORT, &sBuffer, sizeof(sBuffer), &bytesIn, portMAX_DELAY);
  sampleCount = bytesIn / sizeof(sBuffer[0]);

  for (int i = 0; i < sampleCount; i++)
    sBuffer[i] = (sBuffer[i] < MIC_NOISE) ? 0 : sBuffer[i];
  
  return result == ESP_OK;
}


// -------------
// VOLUME LEVELS
// -------------

int vBuffer[PEAKTOPEAK_BUFFER_SIZE];
int vBufferIndex = 0;
int minVol, maxVol = 0;
int vLvl = 0;

int getPeakToPeak() 
{
  if (sampleCount > 0)
  {
    int min = sBuffer[0];
    int max = sBuffer[0];

    for (int i = 1; i < sampleCount; i++)
    {
      if (sBuffer[i] < min)
        min = sBuffer[i];
      else if (max < sBuffer[i])
        max = sBuffer[i];
    }

    return (max - min);
  }

  return 0;
}
void sampleVolumeLevel()
{
  // append value to volume buffer
  vBuffer[vBufferIndex] = getPeakToPeak();
  if (++vBufferIndex >= PEAKTOPEAK_BUFFER_SIZE) 
    vBufferIndex = 0;

  // get volume range of prior frames
  int minLvl = vBuffer[0];
  int maxLvl = vBuffer[0];

  for (int i = 0; i < PEAKTOPEAK_BUFFER_SIZE; i++) {
    if (vBuffer[i] < minLvl)
      minLvl = vBuffer[i];
    else if (maxLvl < vBuffer[i])
      maxLvl = vBuffer[i];
  }

  // enfoce minimum distance between levels
  if ((maxLvl - minLvl) < MIN_PEAKTOPEAK_DISTANCE)
    maxLvl = minLvl + MIN_PEAKTOPEAK_DISTANCE;

  //minVol = (minVol * 63 + minLvl) >> 6; 
  //maxVol = (maxVol * 63 + maxLvl) >> 6; 

  minVol = (minVol * 31 + minLvl) >> 5;
  maxVol = (maxVol * 31 + maxLvl) >> 5;
}
int getVolumeLevel(int range)
{
  vLvl = (vLvl * 7 + getPeakToPeak()) >> 3; // dampen reading
  return range * (vLvl - minVol) / (long)(maxVol - minVol);
}


// ---------------------------
// FAST FOURIER TRANSFORMATION
// ---------------------------

float vReal[SAMPLE_BUFFER_SIZE];
float vImag[SAMPLE_BUFFER_SIZE];
ArduinoFFT<float> _fft = ArduinoFFT<float>(vReal, vImag, SAMPLE_BUFFER_SIZE, SAMPLE_RATE);

void performFFT()
{
  for (int i = 0; i < SAMPLE_BUFFER_SIZE; i++)
  {
    vReal[i] = (i < sampleCount) ? sBuffer[i] : 0;
    vImag[i] = 0;
  }
  _fft.windowing(FFTWindow::Hamming, FFTDirection::Forward);
  _fft.compute(FFTDirection::Forward);
  _fft.complexToMagnitude();
}

#endif