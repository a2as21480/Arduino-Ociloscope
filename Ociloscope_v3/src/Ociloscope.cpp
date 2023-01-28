#include "Ociloscope.h"

void setADC() {
    //ADC 클럭 1MHz, 변환에 13ADC클럭이 소요된다고 알려져있으니 변환속도가 1MHz/13≒77KHz
    sbi(ADCSRA, ADPS2);
    cbi(ADCSRA, ADPS1);
    cbi(ADCSRA, ADPS0);
};

Ociloscope::Ociloscope(Adafruit_SSD1306 *screenPtr) {
    displayPtr = screenPtr;
}

void Ociloscope::drawAxis()
{
    //왼쪽 기준축
    displayPtr->drawLine(GRAPH_X0, GRAPH_Y0, GRAPH_X0, SCREEN_HEIGHT-1, SSD1306_WHITE);
    displayPtr->drawLine(GRAPH_X0, GRAPH_Y0, GRAPH_X0+3, GRAPH_Y0, SSD1306_WHITE);
    displayPtr->drawLine(GRAPH_X0, SCREEN_HEIGHT-1, GRAPH_X0+3, SCREEN_HEIGHT-1, SSD1306_WHITE);

    //오른쪽 기준축
    displayPtr->drawLine(SCREEN_WIDTH-1, GRAPH_Y0, SCREEN_WIDTH-1, SCREEN_HEIGHT-1, SSD1306_WHITE);
    displayPtr->drawLine(SCREEN_WIDTH-1, GRAPH_Y0, SCREEN_WIDTH-4, GRAPH_Y0, SSD1306_WHITE);
    displayPtr->drawLine(SCREEN_WIDTH-1, SCREEN_HEIGHT-1, SCREEN_WIDTH-4, SCREEN_HEIGHT-1, SSD1306_WHITE);

    //가로 점선
    for(int n=0; n<AXIS_X_NUM; n++) {
        for(int i=GRAPH_X0; i<SCREEN_WIDTH-1; i+=7) displayPtr->drawLine(i, GRAPH_Y0+GRAPH_HEIGHT*(n+1.0)/(AXIS_X_NUM+1.0), i+2, GRAPH_Y0+GRAPH_HEIGHT*(n+1.0)/(AXIS_X_NUM+1.0), SSD1306_WHITE);
    }

    //세로 점선
    for(int n=0; n<AXIS_Y_NUM; n++) {
        for(int i=GRAPH_Y0; i<SCREEN_HEIGHT-1; i+=7) displayPtr->drawLine(GRAPH_X0+GRAPH_WIDTH*(n+1)/(AXIS_Y_NUM+1), i, GRAPH_X0+GRAPH_WIDTH*(n+1)/(AXIS_Y_NUM+1), i+2, SSD1306_WHITE);
        displayPtr->drawLine(GRAPH_X0+GRAPH_WIDTH*(n+1)/(AXIS_Y_NUM+1)-1, GRAPH_Y0, GRAPH_X0+GRAPH_WIDTH*(n+1)/(AXIS_Y_NUM+1)+1, GRAPH_Y0, SSD1306_WHITE); //상단 장식
        displayPtr->drawLine(GRAPH_X0+GRAPH_WIDTH*(n+1)/(AXIS_Y_NUM+1)-1, SCREEN_HEIGHT-1, GRAPH_X0+GRAPH_WIDTH*(n+1)/(AXIS_Y_NUM+1)+1, SCREEN_HEIGHT-1, SSD1306_WHITE); //하단 장식
        displayPtr->drawLine(GRAPH_X0+GRAPH_WIDTH*(n+1)/(AXIS_Y_NUM+1), SCREEN_HEIGHT-1, GRAPH_X0+GRAPH_WIDTH*(n+1)/(AXIS_Y_NUM+1), SCREEN_HEIGHT-3, SSD1306_WHITE); //끝부분에 라인이 안 그려졌을 때 허전하지 않기 위한 하단 장식2
    }
};

void Ociloscope::drawInfo(float max, float mid, float min, uint8_t pin, float term, float averge, uint8_t deci = 2) {
    displayPtr->setTextColor(WHITE);
    displayPtr->setTextSize(TEXT_SIZE);

    displayPtr->setCursor(MAX_X0, MAX_Y0);
    displayPtr->print(max, deci);

    displayPtr->setCursor(MID_X0,MID_Y0);
    displayPtr->print(mid, deci);

    displayPtr->setCursor(MIN_X0,MIN_Y0);
    displayPtr->print(min, deci);

    displayPtr->setCursor(PIN_X0,PIN_Y0);
    displayPtr->print("ADC" + String(pin));

    displayPtr->setCursor(TERM_X0,TERM_Y0);
    displayPtr->print(String(term) + "ms"); 

    displayPtr->setCursor(AVG_X0,AVG_Y0);
    displayPtr->print("avg" + String(averge) + "V");
};

void Ociloscope::drawLoading() {
    displayPtr->clearDisplay();
    displayPtr->setTextColor(WHITE);
    displayPtr->setTextSize(3); //글자 18*24
    displayPtr->setCursor(0,0);
    displayPtr->print("Arduino");

    displayPtr->setCursor(0,30);
    displayPtr->setTextSize(2);
    displayPtr->print("Ociloscope");
};

void Ociloscope::drawGuide(){
    displayPtr->setTextColor(WHITE);
    displayPtr->setTextSize(TEXT_SIZE);

    displayPtr->setCursor(MAX_X0,MAX_Y0);
    displayPtr->print("Max");

    displayPtr->setCursor(MID_X0,MID_Y0);
    displayPtr->print("Mid");

    displayPtr->setCursor(MIN_X0,MIN_Y0);
    displayPtr->print("Min");

    displayPtr->setCursor(PIN_X0,PIN_Y0);
    displayPtr->print("Pin");

    displayPtr->setCursor(TERM_X0,TERM_Y0);
    displayPtr->print("Term"); 

    displayPtr->setCursor(AVG_X0,AVG_Y0);
    displayPtr->print("Averge");

    this->drawAxis();
};