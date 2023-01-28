#include "button.h"
volatile unsigned long time0 = 0;
volatile unsigned long time1 = 0;
volatile unsigned long time2 = 0;
volatile uint8_t state_button2 = 0;

//button0 샘플링 간격 변경
void INIT_INT0() {
  cbi(DDRB,PD2); //2번핀 입력으로
  sbi(EIMSK,INT0); //INT0 인터럽트 활성화
  sbi(EICRA,ISC01); //Falling 에서 활성화
  sei(); //set interupt, 인터럽트 전역적으로 활성화
};

ISR(INT0_vect) {
  if(millis()-time0 > 500) {
    intervalLV++;
    intervalLV %= 6; //6단계 제한
    time0 = millis();
  }
};

//button1 ADC 포트 변환
void INIT_INT1() {
  cbi(DDRB,PD3); //3번핀 입력으로
  sbi(EIMSK,INT1); //INT1 인터럽트 활성화
  sbi(EICRA,ISC11); //Falling 에서 활성화
  sei(); //set interupt, 인터럽트 전역적으로 활성화
};

ISR(INT1_vect) { //메인에서 osc.scopePin = portLV 이용
  if(millis()-time1 > 500) {
    portLV++;
    portLV %= 2;
    time1 = millis();
  }
};

//button2 화면 정지
void INIT_PCINT2() {
  cbi(DDRB,PD4); //4번핀 입력으로
  sbi(PCICR,PCIE2); //PCINT2 인터럽트 활성화
  sbi(PCMSK2,PCINT20); //4번핀의 PCINT2 인터럽트 활성화
  sei();
};

ISR(PCINT2_vect) { //메인에서 state_button2이 1인지 감시해서 while 루프 진입
  if(millis()-time2 > 500) {
    state_button2++;
    state_button2 %= 2;
    time2 = millis();
  }
};