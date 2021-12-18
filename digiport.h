#ifndef __DIGIPORT_H__
#define __DIGIPORT_H__

#include "debug.h"

class Digiport {
public:
    void pinMode(int pin, int mode);
    void softPwmCreate(int pin, int duty, int max_duty);
    void digitalWrite(int pin, int mode);
    void softPwmStop(int pin);
    void softPwmWrite(int pin, int duty);

    void displayInit(int channel, int pinClk, int pinDIO);
    void setBrightness(int channel, int brightness, bool on = true);
    void setSegments(int channel, const int segments[], int length = 4, int pos = 0);
    void clear(int channel);
    void showNumberDec(int channel, int num, bool leading_zero = false, int length = 4, int pos = 0);
    void showNumberDecEx(int channel, int num, int dots = 0, bool leading_zero = false, int length = 4, int pos = 0);
    void showNumberHexEx(int channel, int num, int dots = 0, bool leading_zero = false, int length = 4, int pos = 0);
};

#endif