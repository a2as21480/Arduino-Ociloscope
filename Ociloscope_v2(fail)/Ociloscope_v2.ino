/*
Arduino Ociloscope
Begin 2023-01-18
version: 2.0

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

#include "src/Ociloscope.h"
extern volatile uint8_t portLV;
extern uint8_t scopePin;
Ociloscope osc(&oled,SCREEN_WIDTH,SCREEN_HEIGHT,1);
Scope scope0(A0);
Scope scope1(A1, 30*1000, 7.5*1000); //전압 센서 사용

#include "src/button.h"
extern volatile uint8_t state_button2;

void setADC() {
    //ADC 클럭 1MHz, 변환에 13ADC클럭이 소요된다고 알려져있으니 변환속도가 1MHz/13≒77KHz
    sbi(ADCSRA, ADPS2);
    cbi(ADCSRA, ADPS1);
    cbi(ADCSRA, ADPS0);
};

void drawLoading() {
    oled.clearDisplay();
    oled.setTextColor(WHITE);
    oled.setTextSize(3); //글자 18*24
    oled.setCursor(0,0);
    oled.print("Arduino");

    oled.setCursor(0,30);
    oled.setTextSize(2);
    oled.print("Ociloscope");
};

void setup() {
  setADC();
   oled.begin(SSD1306_SWITCHCAPVCC);
  /* 
   drawLoading();
   oled.display();
   delay(2500); */
/*
   oled.clearDisplay();
   osc.drawGuide(1,3);
   oled.display();
   delay(2500); */

   INIT_INT0();
   //INIT_INT1();
   INIT_PCINT2();
   scopePin = A0; 
};

void loop() {
    //버튼 동작
    switch(portLV) {
        case 0:
        scopePin = A0;
        scope0.readData();
        break;

        case 1:
        scopePin = A1;
        scope1.readData();
        break;
    };

    if(state_button2) while(1) if(!state_button2);

    oled.clearDisplay();
    osc.drawData();
    osc.drawAxis(1,3);
    osc.drawGrape();
    oled.display();
};