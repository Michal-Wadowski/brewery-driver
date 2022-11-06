#include <jni.h>
#include "digiport.h"

class Application {
public:
    Application() {
#ifdef REAL_IO
        wiringPiSetup();
#endif
    }
};

Application application = Application();

JavaVM * jvm;
JNIEnv * env;

jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    printf("JNI_OnLoad\n");

    jvm = vm;

    if (vm->GetEnv((void**)&env, JNI_VERSION_1_8) != JNI_OK) {
        return JNI_ERR;
    }

    return JNI_VERSION_1_8;
}
