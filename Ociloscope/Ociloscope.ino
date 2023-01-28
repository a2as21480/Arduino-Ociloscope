/*
Arduino Ociloscope

Begin 2023-01-18

핀 연결
OLED
SCL 13
SDA 11
RES 8
DC 9
(CS핀이 없는 모듈은 1:n 통신 불가)

버튼 2번핀:데이터 수집 시간 간격 변경
INT0 인터럽트 사용, 풀업 저항
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

#include "src\Ociloscope.h"
Ociloscope scope(&oled, A0);

#include <avr/interrupt.h>
volatile uint8_t delayLV = 0;
volatile unsigned long time = 0;
void INIT_INT0() {
  cbi(DDRB,PD2); //2번핀 입력으로
  sbi(EIMSK,INT0); //INT0 인터럽트 활성화
  sbi(EICRA,ISC01); //Falling 에서 활성화
  sei(); //set interupt, 인터럽트 전역적으로 활성화
};

ISR(INT0_vect) {
  if(millis()-time > 500) {
    delayLV++;
    delayLV %= 6; //6단계 제한
    time = millis();
  }
}

void setup() {
    Serial.begin(9600);
    scope.setADC();
    oled.begin(SSD1306_SWITCHCAPVCC);

    //draw loading screen
    oled.clearDisplay();
    oled.setTextColor(WHITE);
    oled.setTextSize(3); //글자 18*24
    oled.setCursor(0,0);
    oled.print("Arduino");

    oled.setCursor(0,30);
    oled.setTextSize(2);
    oled.print("Ociloscope");
    oled.display();
    delay(2500);

    oled.clearDisplay();
    scope.drawGuide();
    oled.display();
    delay(2500);
    oled.clearDisplay();
    oled.display();

    INIT_INT0();
};

void loop() {
    scope.setInterval(delayLV);

    oled.clearDisplay();
    scope.dataRead();
    scope.dataCacu();
    scope.drawAxis();
    scope.drawInfo();
    scope.drawGrape();
    oled.display();
};
