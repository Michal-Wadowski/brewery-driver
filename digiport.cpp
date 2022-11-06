#include "digiport.h"

Digiport::Digiport() {
    memset(virtualPinValues, 0, sizeof(virtualPinValues));

#ifdef REAL_IO
    memset(displayArray, 0, sizeof(displayArray));
#endif
}

#ifdef REAL_IO

void Digiport::pinMode(int pin, int mode) {
    ::pinMode(pin, mode);
}

void Digiport::softPwmCreate(int pin, int initialValue, int pwmRange) {
    ::softPwmCreate(pin, initialValue, pwmRange);
}

void Digiport::digitalWrite(int pin, int value) {
    ::digitalWrite(pin, value);
}

void Digiport::softPwmStop(int pin) {
    ::softPwmStop(pin);
}

void Digiport::softPwmWrite(int pin, int value) {
    virtualPinValues[pin] = value;
    ::softPwmWrite(pin, value);
}

int Digiport::digitalRead(int pin) {
    return ::digitalRead(pin);
}

int Digiport::softPwmRead(int pin) {
    return virtualPinValues[pin];
}

void Digiport::displayInit(int channel, int pinClk, int pinDIO) {
    if (channel < 0 || channel > sizeof(displayArray)) {
        return;
    }
    if (displayArray[channel] == NULL) {
        delete displayArray[channel];
    }
    displayArray[channel] = new TM1637Display(pinClk, pinDIO);
}

void Digiport::setBrightness(int channel, int brightness, bool on) {
    if (channel < 0 || channel > sizeof(displayArray)) {
        return;
    }

    if (displayArray[channel] != NULL) {
        displayArray[channel]->setBrightness(brightness, on);
    }
}

void Digiport::setSegments(int channel, const uint8_t segments[], int length, int pos) {
    if (channel < 0 || channel > sizeof(displayArray)) {
        return;
    }

    if (displayArray[channel] != NULL) {
        displayArray[channel]->setSegments(segments, length, pos);
    }
}

void Digiport::clear(int channel) {
    if (channel < 0 || channel > sizeof(displayArray)) {
        return;
    }

    if (displayArray[channel] != NULL) {
        displayArray[channel]->clear();
    }
}

void Digiport::showNumberDec(int channel, int num, bool leading_zero, int length, int pos) {
    if (channel < 0 || channel > sizeof(displayArray)) {
        return;
    }

    if (displayArray[channel] != NULL) {
        displayArray[channel]->showNumberDec(num, leading_zero, length, pos);
    }
}

void Digiport::showNumberDecEx(int channel, int num, int dots, bool leading_zero, int length, int pos) {
    if (channel < 0 || channel > sizeof(displayArray)) {
        return;
    }

    if (displayArray[channel] != NULL) {
        displayArray[channel]->showNumberDecEx(num, dots, leading_zero, length, pos);
    }
}

void Digiport::showNumberHexEx(int channel, int num, int dots, bool leading_zero, int length, int pos) {
    if (channel < 0 || channel > sizeof(displayArray)) {
        return;
    }

    if (displayArray[channel] != NULL) {
        displayArray[channel]->showNumberHexEx(num, dots, leading_zero, length, pos);
    }
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

void Digiport::setSegments(int channel, const uint8_t segments[], int length, int pos) {
    debug("setSegments %d ", channel);
    
    for (int i = 0; i < length; i++) {
        debug("[%d] ", segments[i]);
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