#include "jni/wadosm_breweryhost_DriverEntry.h"
#include "jni/wadosm_breweryhost_DigiPort.h"
#include <iostream>
#include "digiport.h"

Digiport * digiport = new Digiport();

class Application {
public:
    Application() {
#ifdef REAL_IO
        wiringPiSetup();
#endif
    }
};

Application application = Application();

JNIEXPORT void JNICALL Java_wadosm_breweryhost_DigiPort_pinMode(JNIEnv *, jobject, jint pin, jint mode) {
    digiport->pinMode(pin, mode);
}

JNIEXPORT void JNICALL Java_wadosm_breweryhost_DigiPort_softPwmCreate(
    JNIEnv *, jobject, jint pin, int jinitialValue, int jpwmRange) {
    digiport->softPwmCreate(pin, jinitialValue, jpwmRange);
}

JNIEXPORT void JNICALL Java_wadosm_breweryhost_DigiPort_digitalWrite(JNIEnv *, jobject, jint pin, jint value) {
    digiport->digitalWrite(pin, value);
}

JNIEXPORT void JNICALL Java_wadosm_breweryhost_DigiPort_softPwmStop(JNIEnv *, jobject, jint pin) {
    digiport->softPwmStop(pin);
}

JNIEXPORT void JNICALL Java_wadosm_breweryhost_DigiPort_softPwmWrite(JNIEnv *, jobject, jint pin, jint value) {
    digiport->softPwmWrite(pin, value);
}

JNIEXPORT jint JNICALL Java_wadosm_breweryhost_DigiPort_digitalRead(JNIEnv *, jobject, jint pin) {
    return digiport->digitalRead(pin);
}

JNIEXPORT jint JNICALL Java_wadosm_breweryhost_DigiPort_softPwmRead(JNIEnv *, jobject, jint pin) {
    return digiport->softPwmRead(pin);
}

JNIEXPORT void JNICALL Java_wadosm_breweryhost_DigiPort_displayInit(
    JNIEnv *, jobject, jint channel, jint pinClk, jint pinDIO) {
    digiport->displayInit(channel, pinClk, pinDIO);
}

JNIEXPORT void JNICALL Java_wadosm_breweryhost_DigiPort_setBrightness(
    JNIEnv *, jobject, jint channel, jint brightness, jboolean on) {
    digiport->setBrightness(channel, brightness, on);
}

JNIEXPORT void JNICALL Java_wadosm_breweryhost_DigiPort_setSegments(
    JNIEnv * env, jobject, jint channel, jbyteArray segments, jint length, jint pos) {

    unsigned char isCopy;
    jbyte * byteSegments = env->GetByteArrayElements(segments, &isCopy);
    jsize len = env->GetArrayLength(segments);

    digiport->setSegments(channel, (uint8_t*)byteSegments, length, pos);

    env->ReleaseByteArrayElements(segments, byteSegments, 0);
}

JNIEXPORT void JNICALL Java_wadosm_breweryhost_DigiPort_clear(JNIEnv *, jobject, jint channel) {
    digiport->clear(channel);
}

JNIEXPORT void JNICALL Java_wadosm_breweryhost_DigiPort_showNumberDec(
    JNIEnv *, jobject, jint channel, jint num, jboolean leading_zero, jint length, jint pos) {
    digiport->showNumberDec(channel, num, leading_zero, length, pos);
}

JNIEXPORT void JNICALL Java_wadosm_breweryhost_DigiPort_showNumberDecEx(
    JNIEnv *, jobject, jint channel, jint num, jint dots, jboolean leading_zero, jint length, jint pos) {
    digiport->showNumberDecEx(channel, num, dots, leading_zero, length, pos);
}

JNIEXPORT void JNICALL Java_wadosm_breweryhost_DigiPort_showNumberHexEx(
    JNIEnv *, jobject, jint channel, jint num, jint dots, jboolean leading_zero, jint length, jint pos) {
    digiport->showNumberHexEx(channel, num, dots, leading_zero, length, pos);
}
