#include "jni/wadosm_breweryhost_DriverEntry.h"
#include <iostream>

JNIEXPORT void JNICALL Java_wadosm_breweryhost_DriverEntry_sayHello (JNIEnv *, jobject) {
    std::cout << "Hello world!!!\n";
}