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
    virtual int process(AbstractConnection * ac);
};

int ExternalProcessor::process(AbstractConnection * ac) {
    ac->buff[ac->size_read] = 0;
    debug("data: %s, size: %ld\n", ac->buff, ac->size_read);
    return 0;
};





Digiport digiport = Digiport();

cJSON * pinMode(std::string functionName, cJSON * argumentsArray) {
    if (functionName == "pinMode") {
        debug("function: pinMode\n");
        
        if (cJSON_GetArraySize(argumentsArray) == 2) {
            int pin = cJSON_GetArrayItem(argumentsArray, 0)->valueint;
            int mode = cJSON_GetArrayItem(argumentsArray, 1)->valueint;
            
            digiport.pinMode(pin, mode);

            cJSON * response = cJSON_CreateNull();
            return response;
        }
    }

    return NULL;
}

cJSON * softPwmCreate(std::string functionName, cJSON * argumentsArray) {
    if (functionName == "softPwmCreate") {
        debug("function: softPwmCreate\n");
        
        if (cJSON_GetArraySize(argumentsArray) == 3) {
            int pin = cJSON_GetArrayItem(argumentsArray, 0)->valueint;
            int initialValue = cJSON_GetArrayItem(argumentsArray, 1)->valueint;
            int pwmRange = cJSON_GetArrayItem(argumentsArray, 2)->valueint;
            
            digiport.softPwmCreate(pin, initialValue, pwmRange);

            cJSON * response = cJSON_CreateNull();
            return response;
        }
    }

    return NULL;
}

cJSON * displayInit(std::string functionName, cJSON * argumentsArray) {
    if (functionName == "displayInit") {
        debug("function: displayInit\n");
        
        if (cJSON_GetArraySize(argumentsArray) == 3) {
            int channel = cJSON_GetArrayItem(argumentsArray, 0)->valueint;
            int pinClk = cJSON_GetArrayItem(argumentsArray, 1)->valueint;
            int pinDIO = cJSON_GetArrayItem(argumentsArray, 2)->valueint;
            
            digiport.displayInit(channel, pinClk, pinDIO);

            cJSON * response = cJSON_CreateNull();
            return response;
        }
    }

    return NULL;
}



cJSON * digitalWrite(std::string functionName, cJSON * argumentsArray) {
    if (functionName == "digitalWrite") {
        debug("function: digitalWrite\n");
        
        if (cJSON_GetArraySize(argumentsArray) == 2) {
            int pin = cJSON_GetArrayItem(argumentsArray, 0)->valueint;
            int value = cJSON_GetArrayItem(argumentsArray, 1)->valueint;
            
            digiport.digitalWrite(pin, value);

            cJSON * response = cJSON_CreateNull();
            return response;
        }
    }

    return NULL;
}

cJSON * softPwmWrite(std::string functionName, cJSON * argumentsArray) {
    if (functionName == "softPwmWrite") {
        debug("function: softPwmWrite\n");
        
        if (cJSON_GetArraySize(argumentsArray) == 2) {
            int pin = cJSON_GetArrayItem(argumentsArray, 0)->valueint;
            int value = cJSON_GetArrayItem(argumentsArray, 1)->valueint;
            
            digiport.softPwmWrite(pin, value);

            cJSON * response = cJSON_CreateNull();
            return response;
        }
    }

    return NULL;
}

cJSON * digitalRead(std::string functionName, cJSON * argumentsArray) {
    if (functionName == "digitalRead") {
        debug("function: digitalRead\n");
        
        if (cJSON_GetArraySize(argumentsArray) == 1) {
            int pin = cJSON_GetArrayItem(argumentsArray, 0)->valueint;
            
            int value = digiport.digitalRead(pin);

            cJSON * response = cJSON_CreateNumber(value);
            return response;
        }
    }

    return NULL;
}

cJSON * softPwmRead(std::string functionName, cJSON * argumentsArray) {
    if (functionName == "softPwmRead") {
        debug("function: softPwmRead\n");
        
        if (cJSON_GetArraySize(argumentsArray) == 1) {
            int pin = cJSON_GetArrayItem(argumentsArray, 0)->valueint;
            
            int value = digiport.softPwmRead(pin);

            cJSON * response = cJSON_CreateNumber(value);
            return response;
        }
    }

    return NULL;
}

void addResponseIfAny(cJSON * commandResp, cJSON * response, cJSON * responseArr) {
    if (response != NULL) {
        cJSON_AddItemToObject(commandResp, "response", response);
        cJSON_AddItemToArray(responseArr, commandResp);
    }
}

void executeHost(cJSON * json, AbstractConnection * ac) {
    
    cJSON * commands = cJSON_GetObjectItemCaseSensitive(json, "commands");
    if (commands != NULL) {
        debug("\nexecuteHost {\n");

        cJSON * responseArr = cJSON_CreateArray();

        cJSON * command = NULL;
        cJSON_ArrayForEach(command, commands)
        {
            cJSON * functionNameC = cJSON_GetObjectItemCaseSensitive(command, "function");

            if (functionNameC != NULL) {
                std::string functionName = functionNameC->valuestring;
                cJSON * argumentsArray = cJSON_GetObjectItemCaseSensitive(command, "arguments");
                cJSON * commandResp = cJSON_Duplicate(command, true);
                cJSON * response;

                response = pinMode(functionName, argumentsArray);
                addResponseIfAny(commandResp, response, responseArr);

                response = softPwmCreate(functionName, argumentsArray);
                addResponseIfAny(commandResp, response, responseArr);

                response = displayInit(functionName, argumentsArray);
                addResponseIfAny(commandResp, response, responseArr);

                response = digitalWrite(functionName, argumentsArray);
                addResponseIfAny(commandResp, response, responseArr);

                response = softPwmWrite(functionName, argumentsArray);
                addResponseIfAny(commandResp, response, responseArr);

                response = digitalRead(functionName, argumentsArray);
                addResponseIfAny(commandResp, response, responseArr);

                response = softPwmRead(functionName, argumentsArray);
                addResponseIfAny(commandResp, response, responseArr);
            }
        }

        if (cJSON_GetArraySize(responseArr) > 0) {
            char * responseStr = cJSON_Print(responseArr);

            if (ac != NULL) {
                write(ac->client, responseStr, strlen(responseStr));
            } else {
                debug("%s\n", responseStr);
            }

            free(responseStr);
        }
        cJSON_Delete(responseArr);

        debug("}\n");
    }

   
}



class HostExecutor : public StreamProcessor {
    virtual int process(AbstractConnection * ac);
};

int HostExecutor::process(AbstractConnection * ac) {
    ac->buff[ac->size_read] = 0;

    cJSON *json = cJSON_Parse(ac->buff);

    if (json == NULL) {
        debug("JSON parsing Error\n");
        return -1;
    }

    executeHost(json, ac);

    delete json;
    

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

    // pthread_t custom_processor_thread;
    // pthread_create(&custom_processor_thread, NULL, bluetooth_processor_callback, NULL);







    pthread_t host_executor_thread;
    pthread_create(&host_executor_thread, NULL, host_executor_callback, NULL);

    pthread_join(host_executor_thread, NULL);


    // // Memory check / primitive tests:
    // for (int i = 0; i < 20000000; i++) {
    //     cJSON *json = cJSON_Parse("{\"commands\":[\
    //         {\"function\":\"pinMode\",\"arguments\":[7, 1]},\
    //         {\"function\":\"softPwmCreate\",\"arguments\":[8, 0, 1000]},\
    //         {\"function\":\"displayInit\",\"arguments\":[0, 15, 16]},\
    //         {\"function\":\"digitalWrite\",\"arguments\":[7, 1]},\
    //         {\"function\":\"softPwmWrite\",\"arguments\":[8, 500]},\
    //         {\"function\":\"digitalRead\",\"arguments\":[7]},\
    //         {\"function\":\"softPwmRead\",\"arguments\":[8]}\
    //     ]}");

    //     if (json == NULL) {
    //         debug("JSON parsing Error\n");
    //         return -1;
    //     }

    //     executeHost(json, NULL);

    //     cJSON_Delete(json);
    // }


    
    return 0;
}
