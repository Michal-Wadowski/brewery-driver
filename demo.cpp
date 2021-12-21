#ifdef BLUETOOTH
#define EXT_CHANNEL 22
#define EXT_MODE ConnectionMode::BLUETOOTH_SOCKET
#else
#define EXT_CHANNEL 2222
#define EXT_MODE ConnectionMode::NETWORK_SOCKET
#endif

#define HOST_CHANNEL 1111

#include "debug.h"
#include "connection.h"
#include "socket.h"
#include "digiport.h"

#include "cJSON.h"

#include "stream_processor.h"

class ExternalProcessor : public StreamProcessor {
    virtual int process(char * data, int len);
};

int ExternalProcessor::process(char * data, int len) {
    data[len] = 0;
    debug("data: %s, size: %d\n", data, len);
    return 0;
};




class HostExecutor : public StreamProcessor {
    virtual int process(char * data, int len);
};

int HostExecutor::process(char * data, int len) {
    data[len] = 0;
    debug("data: %s, size: %d\n", data, len);
    return 0;
};



void * bluetooth_processor_callback(void * args) {
    ExternalProcessor ep = ExternalProcessor();
    Socket sock = Socket(EXT_CHANNEL, EXT_MODE);
    sock.run(&ep);
    return NULL;
}

void * host_executor_callback(void * args) {
    HostExecutor he = HostExecutor();
    Socket sock = Socket(HOST_CHANNEL, ConnectionMode::NETWORK_SOCKET);
    sock.run(&he);
    return NULL;
}

int main() {

    // cJSON *json = cJSON_Parse("[\
    // {\"pinMode\": [0, 0]},\
    // {\"softPwmCreate\":[0, 0, 0]},\
    // {\"digitalWrite\":[0, 0]},\
    // {\"softPwmStop\":[0]},\
    // {\"softPwmWrite\":[0, 0]},\
    // {\"displayInit\":[0, 0, 0]},\
    // {\"setBrightness\": [0, 0, true]},\
    // {\"setSegments\":[0, [0, 0, 0, 0], 4, 0]},\
    // {\"clear\":[0]},\
    // {\"showNumberDec\":[0, 0, false, 0, 0]},\
    // {\"showNumberDecEx\":[0, 0, 0, false, 4, 0]},\
    // {\"showNumberHexEx\":[0, 0, 0, false, 4, 0]}\
    // ]");

    // if (json == NULL) {
    //     debug("JSON parsing Error\n");
    //     return -1;
    // }

    // debug("%p\n", json);

    pthread_t custom_processor_thread;
    pthread_create(&custom_processor_thread, NULL, bluetooth_processor_callback, NULL);

    pthread_t host_executor_thread;
    pthread_create(&host_executor_thread, NULL, host_executor_callback, NULL);

    pthread_join(host_executor_thread, NULL);
    
    return 0;
}
