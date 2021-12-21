#ifndef __SOCKET_H__
#define __SOCKET_H__

#include "connection.h"
#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>
#include <list>

class Socket {
public:
    Socket(int channel, ConnectionMode mode);
    void run(StreamProcessor * stream_processor);
protected:
    int socket_handle;
    ConnectionMode mode;

#ifdef BLUETOOTH
    struct sockaddr_rc blue_loc_addr;
#endif
    struct sockaddr_in net_loc_addr;

};

#endif
