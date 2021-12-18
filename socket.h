#ifndef __SOCKET_H__
#define __SOCKET_H__

#include "connection.h"

class Socket {
public:
    Socket(int channel);
    void run(StreamProcessor * stream_processor);
protected:
    int socket_handle;

#ifdef BLUETOOTH
    struct sockaddr_rc loc_addr;
#else
    struct sockaddr_in loc_addr;
#endif
};

#endif
