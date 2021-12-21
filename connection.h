#ifndef __CONNECTION_H__
#define __CONNECTION_H__


#include <string.h>
#include <unistd.h>
#include <cerrno>
#include <stdio.h>

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

enum ConnectionMode {
    BLUETOOTH_SOCKET = 1,
    NETWORK_SOCKET = 2
};

class Connection {
public:
    Connection(int h_socket, StreamProcessor * sp, ConnectionMode mode);
    void accept();

    bool closed;
protected:
    StreamProcessor * stream_processor;
    ConnectionMode mode;
    int h_socket;
    int client;
    ssize_t size_read;
    char buff[100];

    static void *pthread_callback(void *arg);
    void connection_thread_runner();
    int data_recieved();
};

#endif
