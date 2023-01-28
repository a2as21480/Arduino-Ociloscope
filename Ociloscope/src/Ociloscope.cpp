#include "Ociloscope.h"

inline float Ociloscope::convertVol(unsigned int x) {
    return x*(5.0/1023.0);
};

Ociloscope::Ociloscope(Adafruit_SSD1306 *cPtr, uint8_t pin) {
    displayPtr = cPtr;
    _scopePin = pin;
};

void Ociloscope::setADC() {
    //ADC 클럭 1MHz, 변환에 13ADC클럭이 소요된다고 알려져있으니 변환속도가 1MHz/13≒77KHz
    sbi(ADCSRA, ADPS2);
    cbi(ADCSRA, ADPS1);
    cbi(ADCSRA, ADPS0);
};

void Ociloscope::drawGuide() {
    displayPtr->setTextColor(WHITE);
    displayPtr->setTextSize(1);
    this->drawAxis();

    displayPtr->setCursor(0,14);
    displayPtr->print("Max");

    displayPtr->setCursor(0,32);
    displayPtr->print("Mid");

    displayPtr->setCursor(0,50);
    displayPtr->print("Min");

    displayPtr->setCursor(78,0);
    displayPtr->print("Averge");

    displayPtr->setCursor(28,0);
    displayPtr->print("Term"); 
};

void Ociloscope::drawAxis(){
    //왼쪽 기준축
    displayPtr->drawLine(27,63,27,14,SSD1306_WHITE); 
    displayPtr->drawLine(27,63,30,63,SSD1306_WHITE);
    displayPtr->drawLine(27,14,30,14,SSD1306_WHITE);

    //오른쪽 기준축
    displayPtr->drawLine(127,63,127,14,SSD1306_WHITE); 
    displayPtr->drawLine(123,63,127,63,SSD1306_WHITE);
    displayPtr->drawLine(123,14,127,14,SSD1306_WHITE);

    //가로 중심 점선
    for(int i=0; i<100; i+=7) displayPtr->drawLine(27+i,39,29+i,39,SSD1306_WHITE); //시작점에서 100픽셀 이상 떨어진 지점부터 표시를 못하니

    //세로 점선 25픽셀 간격
    for(int n=0; n<78; n+=26) {
        for(int i=0; i<50; i+=7) displayPtr->drawLine(51+n,14+i,51+n,16+i,SSD1306_WHITE);
        displayPtr->drawLine(50+n,14,52+n,14,SSD1306_WHITE); //상단 점선끝 장식
        displayPtr->drawLine(50+n,63,52+n,63,SSD1306_WHITE); //하단 점선끝 장식
        displayPtr->drawLine(51+n,61,51+n,63,SSD1306_WHITE); //하단 점선끝 장식 세로
    }
};

void Ociloscope::dataCacu() {
    unsigned long _sum = 0;
    for(int i=0; i<98; i++) {
        _max = max(_max, _buffer[i]);
        _min = min(_min, _buffer[i]);
        _sum += _buffer[i];
    };

    //단위 변환
    _max = this->convertVol(_max);
    _min = this->convertVol(_min);
    _mid = (_max + _min)/2.0;
    _averge = this->convertVol(_sum/98.0);
    
};

void Ociloscope::drawInfo() {
    displayPtr->setTextColor(WHITE);
    displayPtr->setTextSize(1); //글자크기 6*8
    displayPtr->setCursor(0,14);
    displayPtr->print(_max);

    displayPtr->setCursor(0,32);
    displayPtr->print(_mid);

    displayPtr->setCursor(0,50);
    displayPtr->print(_min);

    displayPtr->setCursor(78,0);
    displayPtr->print("avg" + String(_averge) + "V");

    displayPtr->setCursor(28,0);
    displayPtr->print(String(_term) + "ms");
};

void Ociloscope::drawGrape() {
    int temp = map(_buffer[0],0,1023,63,14);
    for(int i=0; i<98; i++) {
        displayPtr->drawPixel(28+i,map(_buffer[i],0,1023,63,14),WHITE);
        displayPtr->drawLine(28+i,temp,28+i,map(_buffer[i],0,1023,63,14),SSD1306_WHITE);
        temp = map(_buffer[i],0,1023,63,14);
    };
};

void Ociloscope::setInterval(uint8_t intervalLV) {
    if(!intervalLV) {_interval = -1; _term = 1.27;}
    else            {_interval = 5*intervalLV; _term = 5*intervalLV;}
};

void Ociloscope::dataRead() {
    /*
    변환속도 77KHz라 하면, 98개 데이터 수집하는데 1.272727....ms 소요
    1.27, 5, 10, 15, 20, 25까지 6단계로
    */
   if(_interval == -1) for(int i=0; i<98; i++) _buffer[i] = analogRead(_scopePin);
   else {
    for(int i=0; i<98; i++) {
        _buffer[i] = analogRead(_scopePin);
        delayMicroseconds((_interval-1.27)/98.0*1000.0);
    };
   };
};