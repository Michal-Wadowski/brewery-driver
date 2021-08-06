#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>

#include <wiringPi.h>
#include <softPwm.h>

#include <unistd.h>
#include <sys/stat.h>
#include <pthread.h>

#define SHARED_FILE_PATH "/dev/shm/driver.bin"

#define SOUND_PIN 2
#define POWER_PIN 7
#define MAINS_1_PIN 5 
#define MAINS_2_PIN 13

#define MOTOR_1_PIN 19
#define MOTOR_2_PIN 8
#define MOTOR_3_PIN 20

#define MAINS_TIME_CONSTANT 0x28
#define SHARED_FILE_SIZE 9

void dummy() {}
// #define DEBUG(...) printf(__VA_ARGS__)
#define DEBUG(...) dummy(__VA_ARGS__)

typedef enum {
    OK, FAIL
} Status;

typedef struct {
    bool power;
    bool motor1;
    bool motor2;
    bool motor3;
    u_int16_t sound;
    u_int8_t mains1;
    u_int8_t mains2;
} Ports;

typedef enum {
    NOT_INITIALIZED, IDLE, CONTROLLER_LOCK
} Lock;

typedef struct {
    u_int8_t lock;
    Ports ports;
} SharedFile;

void updateFilePrivileges(char * filename) {
    char command[255] = "chmod ugo+rw -R ";
    strcat(command, filename);
    int cmdResult = system(command);
    if (cmdResult == -1) {}
}

void* backgroundTask(void* args) {
    while(1) {
        updateFilePrivileges("/bluetooth");
        delay(100);
    }
    return NULL;
}

Status readSharedFile(SharedFile * sharedFile) {
    if (access(SHARED_FILE_PATH, F_OK) == 0) {
        return FAIL;
    } else {
        sharedFile->lock = NOT_INITIALIZED;
        return OK;
    }
}

void initPorts(Ports * ports) {
    DEBUG("Init ports\n");

    pinMode(POWER_PIN, OUTPUT);    
    digitalWrite(POWER_PIN, 0);

    pinMode(MOTOR_1_PIN, OUTPUT);    
    digitalWrite(MOTOR_1_PIN, 0);

    pinMode(MOTOR_2_PIN, OUTPUT);    
    digitalWrite(MOTOR_2_PIN, 0);

    pinMode(MOTOR_3_PIN, OUTPUT);    
    digitalWrite(MOTOR_3_PIN, 0);

    ports->power = 0;
    ports->motor1 = 0;
    ports->motor2 = 0;
    ports->motor3 = 0;

    pinMode(SOUND_PIN, OUTPUT);    
    digitalWrite(SOUND_PIN, 0);

    pinMode(MAINS_1_PIN, OUTPUT);    
    digitalWrite(MAINS_1_PIN, 0);

    pinMode(MAINS_2_PIN, OUTPUT);    
    digitalWrite(MAINS_2_PIN, 0);

    ports->sound = 0;
    ports->mains1 = 0;
    ports->mains2 = 0;
}

bool isSharedFileWriteReady(SharedFile * sharedFile) {
    if (access(SHARED_FILE_PATH, F_OK) == 0) {

        FILE * fileHandle = fopen(SHARED_FILE_PATH, "rb");
        if (fileHandle == NULL) {
            return false;
        }
        u_int8_t lock;
        int readSize = fread(&lock, sizeof(lock), 1, fileHandle);
        if (readSize != 1) { // if file empty
            DEBUG("File empty\n");
            lock = NOT_INITIALIZED;
        }

        // if lock invalid
        if (lock != NOT_INITIALIZED && lock != IDLE && lock != CONTROLLER_LOCK
        ) {
            DEBUG("NOT_INITIALIZED (%d)\n", lock);
            lock = NOT_INITIALIZED;
        }

        // if size invalid
        fseek(fileHandle, 0, SEEK_END);
        if (ftell(fileHandle) != SHARED_FILE_SIZE) {
            DEBUG("file size: %ld\n", ftell(fileHandle));
            lock = NOT_INITIALIZED;
        }

        fclose(fileHandle);

        sharedFile->lock = lock;

        bool isReady = (lock == IDLE || lock == NOT_INITIALIZED);

        DEBUG("is write ready: %d, lock: %d\n", isReady, lock);

        return isReady;
    } else {
        sharedFile->lock = NOT_INITIALIZED;
        return true;
    }
}

void initSharedFileStruct(Ports * ports, SharedFile * sharedFile) {
    DEBUG("initSharedFileStruct\n");

    sharedFile->lock = IDLE;

    sharedFile->ports = *ports;
}

void dumpSharedFileStruct(SharedFile * sharedFile, FILE * fileHandle) {
    fwrite(&sharedFile->lock, sizeof(sharedFile->lock), 1, fileHandle);

    fwrite(&sharedFile->ports.power, sizeof(sharedFile->ports.power), 1, fileHandle);

    fwrite(&sharedFile->ports.motor1, sizeof(sharedFile->ports.motor1), 1, fileHandle);
    fwrite(&sharedFile->ports.motor2, sizeof(sharedFile->ports.motor2), 1, fileHandle);
    fwrite(&sharedFile->ports.motor3, sizeof(sharedFile->ports.motor3), 1, fileHandle);

    fwrite(&sharedFile->ports.sound, sizeof(sharedFile->ports.sound), 1, fileHandle);

    fwrite(&sharedFile->ports.mains1, sizeof(sharedFile->ports.mains1), 1, fileHandle);
    fwrite(&sharedFile->ports.mains2, sizeof(sharedFile->ports.mains2), 1, fileHandle);
}

Status createSharedFile(SharedFile * sharedFile) {
    DEBUG("Create shared file!!!\n");

    FILE * newFileHandle = fopen(SHARED_FILE_PATH, "w");
    if (newFileHandle == NULL) {
        DEBUG("Cant create file...\n");
        return FAIL;
    }

    sharedFile->lock = IDLE;
    dumpSharedFileStruct(sharedFile, newFileHandle);

    fclose(newFileHandle);

    updateFilePrivileges(SHARED_FILE_PATH);

    return OK;
}

bool isSharedFileReadReady(SharedFile * sharedFile) {
    if (access(SHARED_FILE_PATH, F_OK) == 0) {

        FILE * fileHandle = fopen(SHARED_FILE_PATH, "rb");
        if (fileHandle == NULL) {
            return false;
        }
        u_int8_t lock;
        int readSize = fread(&lock, sizeof(lock), 1, fileHandle);
        if (readSize != 1) {
            return false;
        }
        fclose(fileHandle);

        bool isReady = (lock != CONTROLLER_LOCK && lock != NOT_INITIALIZED);

        DEBUG("is read ready: %d, lock: %d\n", isReady, lock);

        return isReady;
    } else {
        return false;
    }
}

Status readSharedFilePorts(SharedFile * sharedFile) {
    FILE * fileHandle = fopen(SHARED_FILE_PATH, "rb");
    if (fileHandle == NULL) {
        DEBUG("Cant read file...\n");
        return FAIL;
    }

    fseek(fileHandle, sizeof(sharedFile->lock), SEEK_SET);


    if (fread(&sharedFile->ports.power, sizeof(sharedFile->ports.power), 1, fileHandle) != 1) {
        return FAIL;
    }

    if (fread(&sharedFile->ports.motor1, sizeof(sharedFile->ports.motor1), 1, fileHandle) != 1) {
        return FAIL;
    }
    if (fread(&sharedFile->ports.motor2, sizeof(sharedFile->ports.motor2), 1, fileHandle) != 1) {
        return FAIL;
    }
    if (fread(&sharedFile->ports.motor3, sizeof(sharedFile->ports.motor3), 1, fileHandle) != 1) {
        return FAIL;
    }

    if (fread(&sharedFile->ports.sound, sizeof(sharedFile->ports.sound), 1, fileHandle) != 1) {
        return FAIL;
    }

    if (fread(&sharedFile->ports.mains1, sizeof(sharedFile->ports.mains1), 1, fileHandle) != 1) {
        return FAIL;
    }
    if (fread(&sharedFile->ports.mains2, sizeof(sharedFile->ports.mains2), 1, fileHandle) != 1) {
        return FAIL;
    }

    fclose(fileHandle);

    return OK;
}

bool arePortsChanged(Ports * ports, SharedFile * sharedFile) {
    if (sharedFile->ports.power != ports->power) {
        return true;
    }

    if (sharedFile->ports.motor1 != ports->motor1) {
        return true;
    }

    if (sharedFile->ports.motor2 != ports->motor2) {
        return true;
    }

    if (sharedFile->ports.motor3 != ports->motor3) {
        return true;
    }

    if (sharedFile->ports.sound != ports->sound) {
        return true;
    }

    if (sharedFile->ports.mains1 != ports->mains1) {
        return true;
    }

    if (sharedFile->ports.mains2 != ports->mains2) {
        return true;
    }

    return false;
}

void updatePorts(Ports * ports, SharedFile * sharedFile) {
    DEBUG("Updating ports!\n");

    if (sharedFile->ports.power != ports->power) {
        ports->power = sharedFile->ports.power;
        digitalWrite(POWER_PIN, ports->power);
    }

    if (sharedFile->ports.motor1 != ports->motor1) {
        ports->motor1 = sharedFile->ports.motor1;
        digitalWrite(MOTOR_1_PIN, ports->motor1);
    }

    if (sharedFile->ports.motor2 != ports->motor2) {
        ports->motor2 = sharedFile->ports.motor2;
        digitalWrite(MOTOR_2_PIN, ports->motor2);
    }

    if (sharedFile->ports.motor3 != ports->motor3) {
        ports->motor3 = sharedFile->ports.motor3;
        digitalWrite(MOTOR_3_PIN, ports->motor3);
    }

    DEBUG("SOUND: %d, \n", sharedFile->ports.sound);

    if (sharedFile->ports.sound != ports->sound) {
        ports->sound = sharedFile->ports.sound;
        
        softPwmStop(SOUND_PIN);
        softPwmCreate(SOUND_PIN, ports->sound / 2, ports->sound);
    }

    if (sharedFile->ports.mains1 != ports->mains1) {
        ports->mains1 = sharedFile->ports.mains1;

        softPwmStop(MAINS_1_PIN);
        softPwmCreate(MAINS_1_PIN, MAINS_TIME_CONSTANT*ports->mains1, MAINS_TIME_CONSTANT*0xff);
    }

    if (sharedFile->ports.mains2 != ports->mains2) {
        ports->mains2 = sharedFile->ports.mains2;

        softPwmStop(MAINS_2_PIN);
        softPwmCreate(MAINS_2_PIN, MAINS_TIME_CONSTANT*ports->mains2, MAINS_TIME_CONSTANT*0xff);
    }
}   
   
int main() {
    DEBUG("Start\n");

    wiringPiSetup();
    
    pthread_t backgroundTaskId;
    pthread_create(&backgroundTaskId, NULL, backgroundTask, NULL);

    Ports currPorts;
    initPorts(&currPorts);

    SharedFile sharedFile;
    initSharedFileStruct(&currPorts, &sharedFile);

    while(1) {
        
        if (isSharedFileWriteReady(&sharedFile)) {
            if (sharedFile.lock == NOT_INITIALIZED) {
                createSharedFile(&sharedFile);
            }
        }

        if (isSharedFileReadReady(&sharedFile)) {
            Status status = readSharedFilePorts(&sharedFile);

            DEBUG("status: %d, snd: %d\n", status, sharedFile.ports.sound);

            if (arePortsChanged(&currPorts, &sharedFile)) {
                updatePorts(&currPorts, &sharedFile);
            }
        }

        delay(100);
    }

    return 0;
}

