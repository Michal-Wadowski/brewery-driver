/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class wadosm_breweryhost_DigiPort */

#ifndef _Included_wadosm_breweryhost_DigiPort
#define _Included_wadosm_breweryhost_DigiPort
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     wadosm_breweryhost_DigiPort
 * Method:    pinMode
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_wadosm_breweryhost_DigiPortImpl_pinMode
  (JNIEnv *, jobject, jint, jint);

/*
 * Class:     wadosm_breweryhost_DigiPort
 * Method:    softPwmCreate
 * Signature: (III)V
 */
JNIEXPORT void JNICALL Java_wadosm_breweryhost_DigiPortImpl_softPwmCreate
  (JNIEnv *, jobject, jint, jint, jint);

/*
 * Class:     wadosm_breweryhost_DigiPort
 * Method:    digitalWrite
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_wadosm_breweryhost_DigiPortImpl_digitalWrite
  (JNIEnv *, jobject, jint, jint);

/*
 * Class:     wadosm_breweryhost_DigiPort
 * Method:    softPwmStop
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_wadosm_breweryhost_DigiPortImpl_softPwmStop
  (JNIEnv *, jobject, jint);

/*
 * Class:     wadosm_breweryhost_DigiPort
 * Method:    softPwmWrite
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_wadosm_breweryhost_DigiPortImpl_softPwmWrite
  (JNIEnv *, jobject, jint, jint);

/*
 * Class:     wadosm_breweryhost_DigiPort
 * Method:    digitalRead
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_wadosm_breweryhost_DigiPortImpl_digitalRead
  (JNIEnv *, jobject, jint);

/*
 * Class:     wadosm_breweryhost_DigiPort
 * Method:    softPwmRead
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_wadosm_breweryhost_DigiPortImpl_softPwmRead
  (JNIEnv *, jobject, jint);

/*
 * Class:     wadosm_breweryhost_DigiPort
 * Method:    displayInit
 * Signature: (III)V
 */
JNIEXPORT void JNICALL Java_wadosm_breweryhost_DigiPortImpl_displayInit
  (JNIEnv *, jobject, jint, jint, jint);

/*
 * Class:     wadosm_breweryhost_DigiPort
 * Method:    setBrightness
 * Signature: (IIZ)V
 */
JNIEXPORT void JNICALL Java_wadosm_breweryhost_DigiPortImpl_setBrightness
  (JNIEnv *, jobject, jint, jint, jboolean);

/*
 * Class:     wadosm_breweryhost_DigiPort
 * Method:    setSegments
 * Signature: (I[BII)V
 */
JNIEXPORT void JNICALL Java_wadosm_breweryhost_DigiPortImpl_setSegments
  (JNIEnv *, jobject, jint, jbyteArray, jint, jint);

/*
 * Class:     wadosm_breweryhost_DigiPort
 * Method:    clear
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_wadosm_breweryhost_DigiPortImpl_clear
  (JNIEnv *, jobject, jint);

/*
 * Class:     wadosm_breweryhost_DigiPort
 * Method:    showNumberDec
 * Signature: (IIZII)V
 */
JNIEXPORT void JNICALL Java_wadosm_breweryhost_DigiPortImpl_showNumberDec
  (JNIEnv *, jobject, jint, jint, jboolean, jint, jint);

/*
 * Class:     wadosm_breweryhost_DigiPort
 * Method:    showNumberDecEx
 * Signature: (IIIZII)V
 */
JNIEXPORT void JNICALL Java_wadosm_breweryhost_DigiPortImpl_showNumberDecEx
  (JNIEnv *, jobject, jint, jint, jint, jboolean, jint, jint);

/*
 * Class:     wadosm_breweryhost_DigiPort
 * Method:    showNumberHexEx
 * Signature: (IIIZII)V
 */
JNIEXPORT void JNICALL Java_wadosm_breweryhost_DigiPortImpl_showNumberHexEx
  (JNIEnv *, jobject, jint, jint, jint, jboolean, jint, jint);

#ifdef __cplusplus
}
#endif
#endif