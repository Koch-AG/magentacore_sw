#include "Arduino.h"
#include "FrequencBuzzer.h"


bool playing = false;
unsigned long duration = 0;
unsigned long playingTime = 0;
unsigned int frequency = 0;

void BuzzerFrequenc::beginBuzFreq(byte pin, unsigned long dur, unsigned int freq)
{
    ledcAttachPin(pin, 0);
    duration = dur;
    frequency = freq;
    playing = true;
    ledcWriteTone(0, frequency); 
    playingTime = millis() + duration;
}

void BuzzerFrequenc::playBuzFreq()
{
    unsigned long milliseconds = millis();

    if(milliseconds < playingTime)
    {
        return;
    }
    else
    {
        stopBuzFreq();
    }
}

void BuzzerFrequenc::stopBuzFreq()
{
    if(playing)
    {
        ledcWriteTone(0, 0);
        playing = false;
    }
}

bool  BuzzerFrequenc::isPlayingBuzFreq()
{
  return playing;
}