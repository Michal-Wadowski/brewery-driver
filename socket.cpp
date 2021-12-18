#include "socket.h"

Socket::Socket(int channel) {
    memset(&loc_addr, 0, sizeof(loc_addr));

#ifdef BLUETOOTH
    socket_handle = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
    loc_addr.rc_family = AF_BLUETOOTH;
    loc_addr.rc_channel = (uint8_t)channel;
#else
    socket_handle = socket(AF_INET , SOCK_STREAM , 0);
    loc_addr.sin_family = AF_INET;
	loc_addr.sin_addr.s_addr = INADDR_ANY;
	loc_addr.sin_port = htons(channel);
#endif

    int optval = 1;
    setsockopt(socket_handle, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
    setsockopt(socket_handle, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(optval));

    bind(socket_handle, (struct sockaddr *) &loc_addr, sizeof(loc_addr));

    listen(socket_handle, 1);
}

void Socket::run(StreamProcessor * stream_processor) {
    while(1) {
        Connection conn = Connection(socket_handle, stream_processor);
        debug("Connection waiting... (%d)\n", socket_handle);
        conn.accept();
    }
}
