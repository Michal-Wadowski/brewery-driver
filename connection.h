#ifndef __CONNECTION_H__
#define __CONNECTION_H__


#include <string.h>
#include <unistd.h>

#ifdef BLUETOOTH
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
#else
#include <sys/socket.h> 
#include <arpa/inet.h>
#endif

#include <pthread.h>

#include "debug.h"
#include "stream_processor.h"

class Connection {
public:
    Connection(int h_socket, StreamProcessor * sp);
    void accept();
protected:
    StreamProcessor * stream_processor;
    int h_socket;
    int client;
    ssize_t size_read;
    char buff[100];

    static void *pthread_callback(void *arg);
    void connection_thread_runner();
    int data_recieved();
};

#endif
