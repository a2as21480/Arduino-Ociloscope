#ifndef OCILOSCOPE
#define OCILOSCOPE

#include "Arduino.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

extern volatile uint8_t intervalLV; //데이터 측정 간격 레벨
extern volatile uint8_t portLV;

extern uint16_t *buf; //동적 할당 데이터 수집 버퍼
extern uint8_t buf_length;
extern uint8_t scopePin;
extern float _max, _mid, _min, _term, _averge;

class Ociloscope {
    private:
    //screen
    Adafruit_SSD1306 *displayPtr;
    uint16_t _screen_height, _screen_width;

    //graph
    uint16_t _graph_height, _graph_width;
    uint16_t _graph_x0, _graph_y0;

    //UI
    uint8_t _text_size;
    uint16_t _max_x0, _max_y0;
    uint16_t _mid_x0, _mid_y0;
    uint16_t _min_x0, _min_y0;
    uint16_t _pin_x0, _pin_y0;
    uint16_t _term_x0, _term_y0;
    uint16_t _avg_x0, _avg_y0; 

    public:
    Ociloscope(Adafruit_SSD1306 *cPtr, uint16_t screen_width, uint16_t screen_height, uint8_t text_size = 1);
    ~Ociloscope();
    void drawGuide(uint8_t num_x = 1, uint8_t num_y = 3);
    void drawData();
    void drawAxis(uint8_t num_x = 1, uint8_t num_y = 3);
    void drawGrape();
};

class Scope {
    private:
    Ociloscope *oscPtr;
    float R1, R2;
    uint8_t flag = 0;
    uint8_t _pin;

    //ADC값을 전압값으로 변환
    inline double convertVol(uint16_t x);

    public:
    Scope(uint8_t pin);
    Scope(uint8_t pin, float r1, float r2); //전압센서처럼 저항 분압을 사용한 경우
    void readData();
};
#endif