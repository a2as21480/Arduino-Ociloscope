/*
Button for Arduino Ociloscope

button0 샘플링 간격 변경
button1 ADC 포트 변환
button2 화면 정지
*/
//Macro
#ifndef cbi //clear bit
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif

#ifndef sbi //set bit
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

#ifndef BUTTON
#define BUTTON
#include "Arduino.h"
#include "Ociloscope.h"

/*
전역 변수를 여러 파일에서 공유해서 사용하는 경우, 헤더에는 extern을 사용하고, cpp 파일에 정의한다.
헤더 파일은 여기저기서 include 되는데 그때마다 정의되면 multiple defination 오류가 뜬다.
cpp에만 정의해도 되지만, 메뉴판처럼 헤더파일을 열어 무슨 변수와 함수가 있는지 확인하는 편이 좋기 때문에 헤더에도 정의함
컴파일러는 최종적으로 나누어져있는 정의를 모아서 main 함수 위에 붙여넣기 하는 식으로 작동되는 것을 기억하자
*/
extern volatile uint8_t intervalLV;
extern volatile uint8_t portLV;

extern volatile unsigned long time0;
extern volatile unsigned long time10;
extern volatile unsigned long time2;
extern volatile uint8_t state_button2;

void INIT_INT0();
void INIT_INT1();
void INIT_PCINT2();
#endif