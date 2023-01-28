/*
Arduino Ociloscope
Begin 2023-01-18
version: 3.0

최소 화면 크키 128*64

핀 연결
OLED
SCL 13
SDA 11
RES 8
DC 9
(CS핀이 없는 모듈은 1:n 통신 불가)

버튼 2번핀:데이터 수집 시간 간격 변경
INT0 인터럽트 사용, 풀업 저항

버튼 3번핀: ADC 변경
INT1 인터럽트 사용, 풀업 저항

버튼 4번핀: 화면 정지
PCINT2 인터럽트 사용, 풀업 저항
*/

//OLED
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_MOSI 11 //SDA라 표기됨
#define OLED_CLK 13 //SCL라 표기됨
#define OLED_DC 9
#define OLED_CS 10
#define OLED_RESET 8
Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

#include "src/button.h"

#include "src/Ociloscope.h"
Ociloscope scope(&oled);

unsigned int buffer[BUF_LENGTH];

#define A1_R1 30.0
#define A1_R2 7.5 

//이유는 정확히 모르겟지만 inline 함수를 파일을 분리해서 오버로딩하면 undefined reference 오류뜸
inline float  cVol(uint16_t x) {
    return x*(5.0/1023.0);
};

inline float  cVol(uint16_t x, float r1, float r2) {
    return x*(5.0/1023.0)*((r1+r2)/r2);
};

void setup() {
    INIT_INT0();
    INIT_INT1();
    INIT_PCINT2();
    setADC();
    oled.begin(SSD1306_SWITCHCAPVCC);

    oled.clearDisplay();
    scope.drawLoading();
    oled.display();
    delay(2500);

    oled.clearDisplay();
    scope.drawGuide();
    oled.display();
    delay(2500);
}

void loop() {
    uint8_t pin = A0 + button1LV;
    unsigned long Sum = 0;
    float Max = 0.0, Min = 0.0, Mid = 0.0, Term = 0.0, Averge = 0.0;

    //데이터 수집
    if(!button0LV) {
        Term = BUF_LENGTH/77.0;
        for(int i=0; i<BUF_LENGTH; i++) {
            buffer[i] = analogRead(pin);
        }
    }
    else {
        for(int i = button0LV; i<6; i++) if(5*i- BUF_LENGTH/77.0 > 0) {Term = 5*i; button0LV = i; break;} //Term이 양수가 되도록 조정
        Term = 5*button0LV;
        unsigned int delayTerm = (Term - BUF_LENGTH/77.0)/BUF_LENGTH*1000.0;
        for(int i=0; i<BUF_LENGTH; i++) {
            buffer[i] = analogRead(pin);
            delayMicroseconds(delayTerm);
        }
    }

    //데이터 처리&그래프 그리기
    oled.clearDisplay();
    uint16_t pri = map(buffer[0],0,1023,SCREEN_HEIGHT-1,GRAPH_Y0);
    for(int i=0; i<BUF_LENGTH; i++) {
         Max = max(Max, buffer[i]);
         Min = min(Min, buffer[i]);
         Sum += buffer[i];

        uint16_t temp = map(buffer[i],0,1023,SCREEN_HEIGHT-1,GRAPH_Y0);
        oled.drawPixel(GRAPH_X0+1+i, temp, SSD1306_WHITE);
        oled.drawLine(GRAPH_X0+1+i, pri, GRAPH_X0+1+i, temp, SSD1306_WHITE);
        pri = temp;
    }

    switch(pin) {
        case A0:
        Max = cVol(Max);
        Min = cVol(Min);
        Mid = (Max + Min)/2.0;
        Averge = cVol(Sum/BUF_LENGTH);
        scope.drawInfo(Max,Mid,Min,button1LV,Term,Averge);
        break;

        case A1:
        Max = cVol(Max,A1_R1,A1_R2);
        Min = cVol(Min,A1_R1,A1_R2);
        Mid = (Max + Min)/2.0;
        Averge = cVol(Sum/BUF_LENGTH,A1_R1,A1_R2);
        scope.drawInfo(Max,Mid,Min,button1LV,Term,Averge,1);
        break;
    }
    scope.drawAxis();
    oled.display();

    while(button2LV);
}