#pragma once
#include <Arduino.h>

namespace BuzzerFrequenc
{   
    void beginBuzFreq(byte pin, unsigned long dur, unsigned int freq);
    void playBuzFreq();
    void stopBuzFreq();
    bool isPlayingBuzFreq();
};