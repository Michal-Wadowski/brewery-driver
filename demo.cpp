#ifdef BLUETOOTH
#define CHANNEL 22
#else
#define CHANNEL 2222
#endif

#include "debug.h"
#include "connection.h"
#include "socket.h"
#include "digiport.h"

#include "cJSON.h"

#include "stream_processor.h"

class CustomStreamProcessor : public StreamProcessor {
    virtual int process(char * data, int len);
};

int CustomStreamProcessor::process(char * data, int len) {
    data[len] = 0;
    debug("data: %s, size: %d\n", data, len);
    return 0;
};

int main() {

    cJSON *json = cJSON_Parse("[\
    {\"pinMode\": [0, 0]},\
    {\"softPwmCreate\":[0, 0, 0]},\
    {\"digitalWrite\":[0, 0]},\
    {\"softPwmStop\":[0]},\
    {\"softPwmWrite\":[0, 0]},\
    {\"displayInit\":[0, 0, 0]},\
    {\"setBrightness\": [0, 0, true]},\
    {\"setSegments\":[0, [0, 0, 0, 0], 4, 0]},\
    {\"clear\":[0]},\
    {\"showNumberDec\":[0, 0, false, 0, 0]},\
    {\"showNumberDecEx\":[0, 0, 0, false, 4, 0]},\
    {\"showNumberHexEx\":[0, 0, 0, false, 4, 0]}\
    ]");

    if (json == NULL) {
        debug("JSON parsing Error\n");
        return -1;
    }

    debug("%p\n", json);

    CustomStreamProcessor sp = CustomStreamProcessor();

    Socket sock = Socket(CHANNEL);
    sock.run(&sp);
    
    return 0;
}
