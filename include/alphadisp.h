#pragma once
#include <Arduino.h>

class AlphaDispDriver
{
private:
    char data[4];
    bool colon;
    bool dot;
public:
    AlphaDispDriver();
    void begin(void);
    void writeChar(uint8_t pos, char c);
    char getData(uint8_t pos);
    bool getColon();
    bool getDot();
};