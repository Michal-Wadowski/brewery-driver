#include "digiport.h"

Digiport::Digiport() {
    memset(virtualPinValues, 0, sizeof(virtualPinValues));
}

#ifdef REAL_IO

void Digiport::pinMode(int pin, int mode) {
    debug("pinMode %d, %d\n", pin, mode);
}

void Digiport::softPwmCreate(int pin, int initialValue, int pwmRange) {
    debug("softPwmCreate %d, %d, %d\n", pin, initialValue, pwmRange);
    virtualPinValues[pin] = initialValue;
}

void Digiport::digitalWrite(int pin, int value) {
    debug("digitalWrite %d, %d\n", pin, value);
    virtualPinValues[pin] = value;
}

void Digiport::softPwmStop(int pin) {
    debug("softPwmStop %d\n", pin);
}

void Digiport::softPwmWrite(int pin, int value) {
    debug("softPwmWrite %d, %d\n", pin, value);
    virtualPinValues[pin] = value;
}

int Digiport::digitalRead(int pin) {
    debug("digitalRead %d (%d)\n", pin, virtualPinValues[pin]);
    return virtualPinValues[pin];
}

int Digiport::softPwmRead(int pin) {
    debug("softPwmRead %d (%d)\n", pin, virtualPinValues[pin]);
    return virtualPinValues[pin];
}

void Digiport::displayInit(int channel, int pinClk, int pinDIO) {
    debug("displayInit %d, %d, %d\n", channel, pinClk, pinDIO);
}

void Digiport::setBrightness(int channel, int brightness, bool on) {
    debug("setBrightness %d, %d, %d\n", channel, brightness, on);
}

void Digiport::setSegments(int channel, const int segments[], int length, int pos) {
    debug("setSegments %d ", channel);
    
    for (int i = 0; i < length; i++) {
        debug("%d ", segments[i]);
    }

    debug("%d %d\n", length, pos);
}

void Digiport::clear(int channel) {
    debug("clear %d\n", channel);
}

void Digiport::showNumberDec(int channel, int num, bool leading_zero, int length, int pos) {
    debug("showNumberDec %d, %d, %d, %d, %d\n", channel, num, leading_zero, length, pos);
}

void Digiport::showNumberDecEx(int channel, int num, int dots, bool leading_zero, int length, int pos) {
    debug("showNumberDecEx %d, %d, %d, %d, %d, %d\n", channel, num, dots, leading_zero, length, pos);
}

void Digiport::showNumberHexEx(int channel, int num, int dots, bool leading_zero, int length, int pos) {
    debug("showNumberHexEx %d, %d, %d, %d, %d, %d\n", channel, num, dots, leading_zero, length, pos);
}

#else

void Digiport::pinMode(int pin, int mode) {
    debug("pinMode %d, %d\n", pin, mode);
}

void Digiport::softPwmCreate(int pin, int initialValue, int pwmRange) {
    debug("softPwmCreate %d, %d, %d\n", pin, initialValue, pwmRange);
    virtualPinValues[pin] = initialValue;
}

void Digiport::digitalWrite(int pin, int value) {
    debug("digitalWrite %d, %d\n", pin, value);
    virtualPinValues[pin] = value;
}

void Digiport::softPwmStop(int pin) {
    debug("softPwmStop %d\n", pin);
}

void Digiport::softPwmWrite(int pin, int value) {
    debug("softPwmWrite %d, %d\n", pin, value);
    virtualPinValues[pin] = value;
}

int Digiport::digitalRead(int pin) {
    debug("digitalRead %d (%d)\n", pin, virtualPinValues[pin]);
    return virtualPinValues[pin];
}

int Digiport::softPwmRead(int pin) {
    debug("softPwmRead %d (%d)\n", pin, virtualPinValues[pin]);
    return virtualPinValues[pin];
}

void Digiport::displayInit(int channel, int pinClk, int pinDIO) {
    debug("displayInit %d, %d, %d\n", channel, pinClk, pinDIO);
}

void Digiport::setBrightness(int channel, int brightness, bool on) {
    debug("setBrightness %d, %d, %d\n", channel, brightness, on);
}

void Digiport::setSegments(int channel, const int segments[], int length, int pos) {
    debug("setSegments %d ", channel);
    
    for (int i = 0; i < length; i++) {
        debug("%d ", segments[i]);
    }

    debug("%d %d\n", length, pos);
}

void Digiport::clear(int channel) {
    debug("clear %d\n", channel);
}

void Digiport::showNumberDec(int channel, int num, bool leading_zero, int length, int pos) {
    debug("showNumberDec %d, %d, %d, %d, %d\n", channel, num, leading_zero, length, pos);
}

void Digiport::showNumberDecEx(int channel, int num, int dots, bool leading_zero, int length, int pos) {
    debug("showNumberDecEx %d, %d, %d, %d, %d, %d\n", channel, num, dots, leading_zero, length, pos);
}

void Digiport::showNumberHexEx(int channel, int num, int dots, bool leading_zero, int length, int pos) {
    debug("showNumberHexEx %d, %d, %d, %d, %d, %d\n", channel, num, dots, leading_zero, length, pos);
}


#endif