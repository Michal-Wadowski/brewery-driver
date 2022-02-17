#ifndef __DIGIPORT_H__
#define __DIGIPORT_H__

#include "debug.h"
#include "string.h"
#include <stdint.h>

#ifdef REAL_IO
#include <wiringPi.h>
#include <softPwm.h>
#include "TM1637Display.h"
#endif

class Digiport {
public:
    Digiport();

    void pinMode(int pin, int mode);
    void softPwmCreate(int pin, int initialValue, int pwmRange);
    void digitalWrite(int pin, int value);
    void softPwmStop(int pin);
    void softPwmWrite(int pin, int value);

    int digitalRead(int pin);
    int softPwmRead(int pin);

    void displayInit(int channel, int pinClk, int pinDIO);
    void setBrightness(int channel, int brightness, bool on = true);
    void setSegments(int channel, const uint8_t segments[], int length = 4, int pos = 0);
    void clear(int channel);
    void showNumberDec(int channel, int num, bool leading_zero = false, int length = 4, int pos = 0);
    void showNumberDecEx(int channel, int num, int dots = 0, bool leading_zero = false, int length = 4, int pos = 0);
    void showNumberHexEx(int channel, int num, int dots = 0, bool leading_zero = false, int length = 4, int pos = 0);

protected:
    int virtualPinValues[40];

#ifdef REAL_IO
    TM1637Display * displayArray[5];
#endif
};
        
#endif