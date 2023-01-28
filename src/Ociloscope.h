#ifndef OCILOSCOPE
#define OCILOSCOPE

#include "Arduino.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

/*
define 매크로의 문제점: 고대로 치환된 후 컴파일되다는 점에 유의하기, 괄호 필수

예시
#define FIVE 5
#define A FIVE+1
Serial.println(A*2);
의도는 A = 6이라 12의 결과값이지만
컴파일러는
Serial.println(FIVE+1*2);
Serial.println(5+1*2);
Serial.println(7); //최적화: 상수 폴딩
가 되어 7로 출력된다.
*/

#ifndef SCREEN_WIDTH
#define SCREEN_WIDTH 128
#endif

#ifndef SCREEN_HEIGHT
#define SCREEN_HEIGHT 64
#endif

#define TEXT_SIZE 1 //1이면 6*8 2이면 12*16 3이면 18*24 ...

//그래프 시작 커서 좌표
#define GRAPH_X0 (18*TEXT_SIZE+8) //26 //글자와 그래프 사이 여백 9pixel
#define GRAPH_Y0 (6*TEXT_SIZE+5) //11 //글자와 그래프 사이 여백 6pixel

//ADC 포트 정보가 표시되는 커서 위치
#define PIN_X0 0
#define PIN_Y0 0

#define MAX_X0 PIN_X0
#define MAX_Y0 GRAPH_Y0

#define MIN_X0 PIN_X0
#define MIN_Y0 (SCREEN_HEIGHT-1-8*TEXT_SIZE) //55

#define MID_X0 PIN_X0
#define MID_Y0 (MAX_Y0+MIN_Y0)/2 //33

#define AVG_X0 (SCREEN_WIDTH-1-48*TEXT_SIZE) //79
#define AVG_Y0 PIN_Y0

#define TERM_X0 ((PIN_X0+24*TEXT_SIZE+AVG_X0)/2-18*TEXT_SIZE) //33
#define TERM_Y0 PIN_Y0

#define GRAPH_WIDTH (SCREEN_WIDTH-GRAPH_X0) //102
#define GRAPH_HEIGHT (SCREEN_HEIGHT-GRAPH_Y0) //53

#define AXIS_X_NUM 1
#define AXIS_Y_NUM 3

#define BUF_LENGTH (GRAPH_WIDTH-2) //100

//Macro
#ifndef cbi //clear bit
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif

#ifndef sbi //set bit
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

void setADC();

class Ociloscope {
    private:
    Adafruit_SSD1306 *displayPtr;

    public:
    Ociloscope(Adafruit_SSD1306 *screenPtr);
    void drawAxis();
    void drawInfo(float max, float mid, float min, uint8_t pin, float term, float averge, uint8_t deci = 2);
    void drawLoading();
    void drawGuide();
};
#endif