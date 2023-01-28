#include "Ociloscope.h"

volatile uint8_t intervalLV = 0; //데이터 측정 간격 레벨
volatile uint8_t portLV = 0;

uint16_t *buf; //동적 할당 데이터 수집 버퍼
uint8_t scopePin;
uint8_t buf_length = 0;
float _max = 0.0, _mid = 0.0, _min = 0.0, _term = 0.0, _averge = 0.0;

Ociloscope::Ociloscope(Adafruit_SSD1306 *cPtr, uint16_t screen_width, uint16_t screen_height, uint8_t text_size = 1) {
    displayPtr = cPtr;
    _screen_height = screen_height;
    _screen_width = screen_width;
    _text_size = text_size;

    _graph_x0 = 18*_text_size+9; //글자와 그래프 사이 여백 10pixel
    _graph_y0 = 6*_text_size+5; //글자와 그래프 사이 여백 6pixel

    _pin_x0 = 0;
    _pin_y0 = 0;

    _max_x0 = _pin_x0;
    _max_y0 = _graph_y0;

    _min_x0 = _pin_x0;
    _min_y0 = screen_height-1-8*_text_size;

    _mid_x0 = _pin_x0;
    _mid_y0 = (_max_y0+_min_y0)/2-1;

    _avg_x0 = screen_width-48*_text_size-1;
    _avg_y0 = _pin_y0;

    _term_x0 = (_pin_x0+24*_text_size+ _avg_x0)/2 - 18*_text_size -1;
    _term_y0 = _pin_y0;

    _graph_width = _screen_width-_graph_x0;
    _graph_height = _screen_height-_graph_y0;

    buf_length = _graph_width;
    buf = new uint16_t[buf_length];
};

Ociloscope::~Ociloscope() {
    if(buf) {
        delete[] buf;
        buf = NULL;
    }
};

void Ociloscope::drawGuide(uint8_t num_x = 1, uint8_t num_y = 3) {
    this->drawAxis(num_x, num_y);
    displayPtr->setTextColor(WHITE);
    displayPtr->setTextSize(_text_size);

    displayPtr->setCursor(_max_x0,_max_y0);
    displayPtr->print("Max");

    displayPtr->setCursor(_mid_x0,_mid_y0);
    displayPtr->print("Mid");

    displayPtr->setCursor(_min_x0,_min_y0);
    displayPtr->print("Min");

    displayPtr->setCursor(_pin_x0,_pin_y0);
    displayPtr->print("Pin");

    displayPtr->setCursor(_term_x0,_term_y0);
    displayPtr->print("Term"); 

    displayPtr->setCursor(_avg_x0,_avg_y0);
    displayPtr->print("Averge");
};

void Ociloscope::drawData() {
    displayPtr->setTextColor(WHITE);
    displayPtr->setTextSize(_text_size);

    displayPtr->setCursor(_max_x0,_max_y0);
    displayPtr->print(_max);

    displayPtr->setCursor(_mid_x0,_mid_y0);
    displayPtr->print(_mid);

    displayPtr->setCursor(_min_x0,_min_y0);
    displayPtr->print(_min);

    displayPtr->setCursor(_pin_x0,_pin_y0);
    displayPtr->print("ADC" + String(portLV));

    displayPtr->setCursor(_term_x0,_term_y0);
    displayPtr->print(String(_term) + "ms"); 

    displayPtr->setCursor(_avg_x0,_avg_y0);
    displayPtr->print("avg" + String(_averge) + "V");
};

void Ociloscope::drawAxis(uint8_t num_x = 1, uint8_t num_y = 3) {
    //왼쪽 기준축
    displayPtr->drawLine(_graph_x0, _graph_y0, _graph_x0, _screen_height-1, SSD1306_WHITE);
    displayPtr->drawLine(_graph_x0, _graph_y0, _graph_x0+3, _graph_y0, SSD1306_WHITE);
    displayPtr->drawLine(_graph_x0, _screen_height-1, _graph_x0+3, _screen_height-1, SSD1306_WHITE);

    //오른쪽 기준축
    displayPtr->drawLine(_screen_width-1, _graph_y0, _screen_width-1, _screen_height-1, SSD1306_WHITE);
    displayPtr->drawLine(_screen_width-1, _graph_y0, _screen_width-4, _graph_y0, SSD1306_WHITE);
    displayPtr->drawLine(_screen_width-1, _screen_height-1, _screen_width-4, _screen_height-1, SSD1306_WHITE);

    //가로 점선
    for(int n=0; n<num_x; n++) {
        for(int i=_graph_x0; i<_screen_width-1; i+=7) displayPtr->drawLine(i, _graph_y0+_graph_height*(n+1.0)/(num_x+1.0), i+2, _graph_y0+_graph_height*(n+1.0)/(num_x+1.0), SSD1306_WHITE);
    }

    //세로 점선
    for(int n=0; n<num_y; n++) {
        for(int i=_graph_y0; i<_screen_height-1; i+=7) displayPtr->drawLine(_graph_x0+_graph_width*(n+1)/(num_y+1), i, _graph_x0+_graph_width*(n+1)/(num_y+1), i+2, SSD1306_WHITE);
        displayPtr->drawLine(_graph_x0+_graph_width*(n+1)/(num_y+1)-1, _graph_y0, _graph_x0+_graph_width*(n+1)/(num_y+1)+1, _graph_y0, SSD1306_WHITE); //상단 장식
        displayPtr->drawLine(_graph_x0+_graph_width*(n+1)/(num_y+1)-1, _screen_height-1, _graph_x0+_graph_width*(n+1)/(num_y+1)+1, _screen_height-1, SSD1306_WHITE); //하단 장식
        displayPtr->drawLine(_graph_x0+_graph_width*(n+1)/(num_y+1), _screen_height-1, _graph_x0+_graph_width*(n+1)/(num_y+1), _screen_height-3, SSD1306_WHITE); //끝부분에 라인이 안 그려졌을 때 허전하지 않기 위한 하단 장식2
    }
};

void Ociloscope::drawGrape() {
    //int temp = map(buf[0],0,1023,_screen_height-1,_graph_y0);
    for(int i=0; i<buf_length; i++) {
        displayPtr->drawPixel(_graph_x0+i, map(buf[0],0,1023,_screen_height-1,_graph_y0), WHITE);
        //displayPtr->drawLine(_graph_x0+i, temp, _graph_x0+i, map(buf[0],0,1023,_screen_height-1,_graph_y0), SSD1306_WHITE);
        //temp = map(buf[0],0,1023,_screen_height-1,_graph_y0);
    }
};

inline double Scope::convertVol(uint16_t x) {
    if(flag) return x*5.0/1023.0*(R1+R2)/R2;
    else     return x*5.0/1023.0;
};

Scope::Scope(uint8_t pin) {
    _pin = pin;
    R1 = 0.0;
    R2 = 0.0;
    flag = 0;
};

Scope::Scope(uint8_t pin, float r1, float r2) {
    _pin = pin;
    R1 = r1;
    R2 = r2;
    flag = 1;
};

/*
    분주비 16이면 ADC 클럭 16Mhz/16 = 1MHz
    변환에 13 ADC 클럭 정도 소요되니 변환속도 1MHz/13 ≒ 77KHz 
*/ 
void Scope::readData() {
   //데이터 샘플링 간격 확인
   if(!intervalLV) _term = buf_length/77.0;
   else for(int i = intervalLV; i<6; i++) if(5*i- buf_length/77.0 > 0) {_term = 5*i; intervalLV = i; break;}

   unsigned long sum = 0;
   float Max = 0.0, Min = 0.0;
   if(!intervalLV) for(int i=0; i<buf_length; i++) buf[i] = analogRead(_pin);
   else {
    for(int i=0; i<buf_length; i++) {
        buf[i] = analogRead(_pin);
        Max = max(Max, buf[i]);
        Min = min(Min, buf[i]);
        sum += buf[i];
        delayMicroseconds((5*intervalLV - buf_length/77.0)/buf_length*1000.0);
    }
   }

    //단위 변환
    _max = convertVol(Max);
    _min = convertVol(Min);
    _mid = convertVol((Max + Min)/2.0);
    _averge = convertVol(sum/buf_length);
};
