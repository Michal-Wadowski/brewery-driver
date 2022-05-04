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

#include <arpa/inet.h>
#include <sstream>

void executeBluetooth(cJSON * json, AbstractConnection * ac) {
    
    cJSON * command = cJSON_GetObjectItemCaseSensitive(json, "command");
    if (command != NULL) {

        cJSON * body = cJSON_Duplicate(json, true);
        cJSON_DeleteItemFromObject(body, "command");
        char * requestBody = cJSON_Print(body);
        cJSON_Delete(body);

        
        

        std::string commandName = command->valuestring;

        debug("bluetooth command: %s\n", commandName.c_str());

        std::size_t dotPos = commandName.find(".");
        
        std::string path;
        if (dotPos > 0 && dotPos != std::string::npos)
        {
            std::string controller = commandName.substr(0, dotPos);
            controller[0] = std::tolower(controller[0]);
            std::string secondPart = commandName.substr(dotPos + 1);
            path = "/" + controller + "/" + secondPart;
        } else {
            std::size_t slashPos = commandName.find("/");
            if (slashPos == 0) {
                path = commandName;
            }
        }

        if (path.size() > 0) {
            std::string bodySizeStr = std::to_string(strlen(requestBody));
            std::string request = "POST " + 
                path + " HTTP/1.0\r\nHost: localhost\r\nContent-type: application/json\r\nContent-length: " +
                bodySizeStr +
                "\r\nConnection: Close\r\n\r\n" +
                requestBody;

            free(requestBody);


            int sock = 0, valread;
            struct sockaddr_in serv_addr;
            char buffer[1024] = {0};
            if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
            {
                perror("\n Socket creation error \n");
                return;
            }
        
            serv_addr.sin_family = AF_INET;
            serv_addr.sin_port = htons(8080);
            
            // Convert IPv4 and IPv6 addresses from text to binary form
            if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) 
            {
                close(sock);
                perror("\nInvalid address/ Address not supported \n");
                return;
            }
        
            if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
            {
                close(sock);
                perror("\nConnection Failed \n");
                return;
            }
            
            int req_written = write(sock , request.c_str() , strlen(request.c_str()));
            
            valread = recv( sock , buffer, 1024, 0);
            close(sock);

            std::string bufferStr = buffer;
            std::size_t bodyPos = bufferStr.find("\r\n\r\n");

            if (bodyPos > 0 && bodyPos != std::string::npos) {
                std::string responseBody = bufferStr.substr(bodyPos + 4) + "\n";

                int resp_written = write(ac->client, responseBody.c_str(), responseBody.size());
            }
        } else {
            free(requestBody);
        }
    }
}



class ExternalProcessor : public StreamProcessor {
    virtual int process(AbstractConnection * ac);
};

int ExternalProcessor::process(AbstractConnection * ac) {
    ac->buff[ac->size_read] = 0;

    std::stringstream ss(ac->buff);
    std::string part;

    while(std::getline(ss, part, '\n')){
        cJSON *json = cJSON_Parse(part.c_str());

        if (json == NULL) {
            debug("JSON parsing Error\n");
            return -1;
        }

        executeBluetooth(json, ac);

        delete json;
    }
    
    return 0;
};





Digiport driverDigiport = Digiport();

cJSON * pinMode(std::string functionName, cJSON * argumentsArray) {
    if (functionName == "pinMode") {
        debug("function: pinMode\n");
        
        if (cJSON_GetArraySize(argumentsArray) == 2) {
            int pin = cJSON_GetArrayItem(argumentsArray, 0)->valueint;
            int mode = cJSON_GetArrayItem(argumentsArray, 1)->valueint;
            
            driverDigiport.pinMode(pin, mode);

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
            
            driverDigiport.softPwmCreate(pin, initialValue, pwmRange);

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
            
            driverDigiport.displayInit(channel, pinClk, pinDIO);

            cJSON * response = cJSON_CreateNull();
            return response;
        }
    }

    return NULL;
}

cJSON * setBrightness(std::string functionName, cJSON * argumentsArray) {
    if (functionName == "setBrightness") {
        debug("function: setBrightness\n");
        
        if (cJSON_GetArraySize(argumentsArray) == 3) {
            int channel = cJSON_GetArrayItem(argumentsArray, 0)->valueint;
            int brightness = cJSON_GetArrayItem(argumentsArray, 1)->valueint;
            bool on = cJSON_IsTrue(cJSON_GetArrayItem(argumentsArray, 2));
            
            driverDigiport.setBrightness(channel, brightness, on);

            cJSON * response = cJSON_CreateNull();
            return response;
        }
    }

    return NULL;
}

cJSON * showNumberDec(std::string functionName, cJSON * argumentsArray) {
    if (functionName == "showNumberDec") {
        debug("function: showNumberDec\n");
        
        if (cJSON_GetArraySize(argumentsArray) == 5) {
            int channel = cJSON_GetArrayItem(argumentsArray, 0)->valueint;
            int num = cJSON_GetArrayItem(argumentsArray, 1)->valueint;
            bool leading_zero = cJSON_IsTrue(cJSON_GetArrayItem(argumentsArray, 2));
            int length = cJSON_GetArrayItem(argumentsArray, 3)->valueint;
            int pos = cJSON_GetArrayItem(argumentsArray, 4)->valueint;
            
            driverDigiport.showNumberDec(channel, num, leading_zero, length, pos);

            cJSON * response = cJSON_CreateNull();
            return response;
        }
    }

    return NULL;
}

cJSON * showNumberDecEx(std::string functionName, cJSON * argumentsArray) {
    if (functionName == "showNumberDecEx") {
        debug("function: showNumberDecEx\n");
        
        if (cJSON_GetArraySize(argumentsArray) == 6) {
            int channel = cJSON_GetArrayItem(argumentsArray, 0)->valueint;
            int num = cJSON_GetArrayItem(argumentsArray, 1)->valueint;
            int dots = cJSON_GetArrayItem(argumentsArray, 2)->valueint;
            bool leading_zero = cJSON_IsTrue(cJSON_GetArrayItem(argumentsArray, 3));
            int length = cJSON_GetArrayItem(argumentsArray, 4)->valueint;
            int pos = cJSON_GetArrayItem(argumentsArray, 5)->valueint;
            
            driverDigiport.showNumberDecEx(channel, num, dots, leading_zero, length, pos);

            cJSON * response = cJSON_CreateNull();
            return response;
        }
    }

    return NULL;
}

cJSON * clear(std::string functionName, cJSON * argumentsArray) {
    if (functionName == "clear") {
        debug("function: clear\n");
        
        if (cJSON_GetArraySize(argumentsArray) == 1) {
            int channel = cJSON_GetArrayItem(argumentsArray, 0)->valueint;
            
            driverDigiport.clear(channel);

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
            
            driverDigiport.digitalWrite(pin, value);

            cJSON * response = cJSON_CreateNull();
            return response;
        }
    }

    return NULL;
}

cJSON * softPwmWrite(std::string functionName, cJSON * argumentsArray) {
    if (functionName == "softPwmWrite") {
    
        if (cJSON_GetArraySize(argumentsArray) == 2) {
            int pin = cJSON_GetArrayItem(argumentsArray, 0)->valueint;
            int value = cJSON_GetArrayItem(argumentsArray, 1)->valueint;
            
            driverDigiport.softPwmWrite(pin, value);

            cJSON * response = cJSON_CreateNull();
            return response;
        }
    }

    return NULL;
}

cJSON * digitalRead(std::string functionName, cJSON * argumentsArray) {
    if (functionName == "digitalRead") {        
        if (cJSON_GetArraySize(argumentsArray) == 1) {
            int pin = cJSON_GetArrayItem(argumentsArray, 0)->valueint;
            
            int value = driverDigiport.digitalRead(pin);

            cJSON * response = cJSON_CreateNumber(value);
            return response;
        }
    }

    return NULL;
}

cJSON * softPwmRead(std::string functionName, cJSON * argumentsArray) {
    if (functionName == "softPwmRead") {
        if (cJSON_GetArraySize(argumentsArray) == 1) {
            int pin = cJSON_GetArrayItem(argumentsArray, 0)->valueint;
            
            int value = driverDigiport.softPwmRead(pin);

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

                response = digitalWrite(functionName, argumentsArray);
                addResponseIfAny(commandResp, response, responseArr);

                response = softPwmWrite(functionName, argumentsArray);
                addResponseIfAny(commandResp, response, responseArr);

                response = digitalRead(functionName, argumentsArray);
                addResponseIfAny(commandResp, response, responseArr);

                response = softPwmRead(functionName, argumentsArray);
                addResponseIfAny(commandResp, response, responseArr);

                response = displayInit(functionName, argumentsArray);
                addResponseIfAny(commandResp, response, responseArr);

                response = setBrightness(functionName, argumentsArray);
                addResponseIfAny(commandResp, response, responseArr);

                response = showNumberDec(functionName, argumentsArray);
                addResponseIfAny(commandResp, response, responseArr);

                response = showNumberDecEx(functionName, argumentsArray);
                addResponseIfAny(commandResp, response, responseArr);

                response = clear(functionName, argumentsArray);
                addResponseIfAny(commandResp, response, responseArr);
                
            }
        }

        if (cJSON_GetArraySize(responseArr) > 0) {
            char * responseStr = cJSON_PrintUnformatted(responseArr);

            std::string respStr = responseStr;
            respStr +=  + "\n";
            free(responseStr);

            if (ac != NULL) {
                write(ac->client, respStr.c_str(), strlen(respStr.c_str()));
            } else {
                debug("%s\n", responseStr);
            }

            
        }
        cJSON_Delete(responseArr);
    }
}



class HostExecutor : public StreamProcessor {
    virtual int process(AbstractConnection * ac);
};

int HostExecutor::process(AbstractConnection * ac) {
    ac->buff[ac->size_read] = 0;

    std::stringstream ss(ac->buff);
    std::string part;

    while(std::getline(ss, part, '\n')){

        cJSON *json = cJSON_Parse(part.c_str());

        if (json == NULL) {
            debug("JSON parsing Error\n");
            return -1;
        }

        executeHost(json, ac);

        delete json;
    }
    
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
#ifdef REAL_IO
    wiringPiSetup();
#endif

    pthread_t custom_processor_thread;
    pthread_create(&custom_processor_thread, NULL, bluetooth_processor_callback, NULL);


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

    // for (int i = 0; i < 20000000; i++) {

    //     char data[] = "{\"command\":\"Brewing.getBrewingState\",\"commandId\":3}";
    //     cJSON *json = cJSON_Parse(data);

    //     if (json == NULL) {
    //         debug("JSON parsing Error\n");
    //         return -1;
    //     }

    //     executeBluetooth(json, NULL);

    //     cJSON_Delete(json);

    //     usleep(100000);
    // }




    // char data[] = "{\"command\":\"Brewing.getBrewingState\",\"commandId\":32}{\"command\":\"Brewing.getBrewingState\",\"commandId\":33}";
    // cJSON *json = cJSON_Parse(data);
    // if (json == NULL) {
    //     debug("JSON parsing Error\n");
    //     return -1;
    // }
    
    return 0;
}
