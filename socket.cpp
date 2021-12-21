#include "socket.h"

Socket::Socket(int channel, ConnectionMode mode) {
    this->mode = mode;

    socket_handle = -1;

#ifdef BLUETOOTH
    if (mode == ConnectionMode::BLUETOOTH_SOCKET) {
        debug("Bluetooth listening at channel %d\n", channel);
        memset(&blue_loc_addr, 0, sizeof(blue_loc_addr));
        socket_handle = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
        blue_loc_addr.rc_family = AF_BLUETOOTH;
        blue_loc_addr.rc_channel = (uint8_t)channel;
    }
#endif

    if (mode == ConnectionMode::NETWORK_SOCKET) {
        debug("Network listening at port %d\n", channel);
        memset(&net_loc_addr, 0, sizeof(net_loc_addr));
        socket_handle = socket(AF_INET , SOCK_STREAM , 0);
        net_loc_addr.sin_family = AF_INET;
        net_loc_addr.sin_addr.s_addr = INADDR_ANY;
        net_loc_addr.sin_port = htons(channel);
    }


    if (socket_handle != -1) {
        int optval = 1;
        setsockopt(socket_handle, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
        setsockopt(socket_handle, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(optval));

#ifdef BLUETOOTH
        if (mode == ConnectionMode::BLUETOOTH_SOCKET) {
            bind(socket_handle, (struct sockaddr *) &blue_loc_addr, sizeof(blue_loc_addr));
        }
#endif
        if (mode == ConnectionMode::NETWORK_SOCKET) {
            bind(socket_handle, (struct sockaddr *) &net_loc_addr, sizeof(net_loc_addr));
        }

        listen(socket_handle, 1);

    } else {
        perror("Connection listening failed\n");
    }
}

void clearConnections(std::list<Connection*> * connQueue) {
    std::list<Connection*>::iterator it = connQueue->begin();
    
    while(it != connQueue->end()) {
        Connection * conn = *it;

        if (conn->closed) {
            delete (*it);
            connQueue->erase(it++);
        } else {                
            it++;
        }
    }
}

void Socket::run(StreamProcessor * stream_processor) {
    std::list<Connection*> * connQueue = new std::list<Connection*>();

    while(socket_handle != -1) {
        Connection * conn = new Connection(socket_handle, stream_processor, mode);
        debug("Connection waiting... (%d)\n", socket_handle);
        conn->accept();
        connQueue->push_back(conn);
        debug("socket: prepare next connection\n");

        clearConnections(connQueue);
    }

    delete connQueue;
}

