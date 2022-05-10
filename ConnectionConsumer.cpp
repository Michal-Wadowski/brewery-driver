#include "jni/wadosm_breweryhost_ConnectionConsumer.h"

#include <pthread.h>
#include <unistd.h>
#include <list>

#include "debug.h"
#include "connection.h"
#include "socket.h"
#include "stream_processor.h"


extern JavaVM * jvm;
extern JNIEnv * env;

jobject * pobject;

pthread_mutex_t lock;

int cnt = 0;


#ifdef BLUETOOTH
#define EXT_CHANNEL 22
#define EXT_MODE ConnectionMode::BLUETOOTH_SOCKET
#else
#define EXT_CHANNEL 2222
#define EXT_MODE ConnectionMode::NETWORK_SOCKET
#endif

jclass clz;

const char * processMessage(char * message) {
    debug("#message: %s\n", message);

    debug("#pthread_mutex_lock\n");
    pthread_mutex_lock(&lock);

    debug("#AttachCurrentThread\n");
    jvm->AttachCurrentThread((void**)&env, NULL);

    jmethodID midStr = env->GetMethodID(clz, "receivedMessage", "(Ljava/lang/String;)Ljava/lang/String;");
    debug("#methodID: %p\n", midStr);

    jstring jmessage = env->NewStringUTF(message);
    debug("#jmessage: %p\n", jmessage);

    debug("CallObjectMethod()");
    jobject rv = env->CallObjectMethod(*pobject, midStr, jmessage);
    debug("#jobject: %p\n", rv);

    const char * response = env->GetStringUTFChars((jstring)rv, NULL);
    debug("#response: %s\n", response);

    debug("#DetachCurrentThread\n");
    jvm->DetachCurrentThread();

    debug("#pthread_mutex_unlock\n");
    pthread_mutex_unlock(&lock);

    return response;
}

class ExternalProcessor : public StreamProcessor {
    virtual int process(AbstractConnection * ac);
};

int ExternalProcessor::process(AbstractConnection * ac) {
    ac->buff[ac->size_read] = 0;

    debug("#processMessage\n");
    const char * response = processMessage(ac->buff);

    debug("#write response\n");
    write(ac->client, response, strlen(response));
    fsync(ac->client);

    return 0;
};

void * bluetooth_processor_callback(void * args) {
    pobject = (jobject *)args;

    debug("#bluetooth_processor_callback: %p\n", bluetooth_processor_callback);

    ExternalProcessor ep = ExternalProcessor();
    debug("#Socket\n");
    Socket sock = Socket(EXT_CHANNEL, EXT_MODE);
    debug("#run\n");
    sock.run(&ep);
    return NULL;
}

void * bluetooth_driver_processor_callback(void * args) {
    pobject = (jobject *)args;

    char buffer[256];
    sprintf(buffer, "Hello World!!!: %d", cnt++);

    const char * response = processMessage(buffer);

    debug("native response: %s\n", response);

    return NULL;
}

JNIEXPORT void JNICALL Java_wadosm_breweryhost_ConnectionConsumer_attachListener(JNIEnv * env, jobject object) {
    debug("#pthread_mutex_init\n");
    pthread_mutex_init(&lock, NULL);

    clz = env->FindClass("wadosm/breweryhost/ConnectionConsumer");
    debug("#class: %p\n", clz);

    debug("#Java_wadosm_breweryhost_ConnectionConsumer_attachListener\n");

    pthread_t custom_processor_thread;
    pthread_create(&custom_processor_thread, NULL, bluetooth_processor_callback, &object);
//    pthread_create(&custom_processor_thread, NULL, bluetooth_driver_processor_callback, &object);
    pthread_join(custom_processor_thread, NULL);
}