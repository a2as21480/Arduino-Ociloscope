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

/*
전역 변수를 여러 파일에서 공유해서 사용하는 경우, 헤더에는 extern을 사용하고, cpp 파일에 정의한다.
헤더 파일은 여기저기서 include 되는데 그때마다 정의되면 multiple defination 오류가 뜬다.
cpp 에만 정의하면 include할 때마다 그 파일에서 extern을 남발해야함, 헤더에서 extern이 미리 정의됨
컴파일러는 최종적으로 나누어져있는 정의를 모아서 main 함수 위에 붙여넣기 하는 식으로 작동되는 것을 기억하자
*/

extern volatile uint8_t button0LV; //샘플링 간격 변경
extern volatile uint8_t button1LV; //ADC 핀 변경
extern volatile uint8_t button2LV; //화면 정지

void INIT_INT0(); //button0 설정
void INIT_INT1(); //button1 설정
void INIT_PCINT2(); //button2 설정
#endif