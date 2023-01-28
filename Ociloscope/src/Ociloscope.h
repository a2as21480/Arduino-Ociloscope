#ifndef OCILOSCOPE
#define OCILOSCOPE

#include "Arduino.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#ifndef cbi //clear bit
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif

#ifndef sbi //set bit
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

class Ociloscope {
    private:
    Adafruit_SSD1306 *displayPtr;
    unsigned int _buffer[98]; //데이터 저장 버퍼
    float _max=0.0, _min=0.0, _mid=0.0, _averge=0.0, _term=1.27;
    uint8_t _scopePin;
    int8_t _interval = -1;

    inline float convertVol(unsigned int x); //ADC값을 전압값으로 변환

    public:
    Ociloscope(Adafruit_SSD1306 *cPtr, uint8_t pin);
    void setADC();
    void drawGuide();

    void setInterval(uint8_t intervalLV);
    void dataRead();
    void dataCacu(); //max, min, mid, averge 계산

    void drawAxis();
    void drawInfo();
    void drawGrape();
};

#endif