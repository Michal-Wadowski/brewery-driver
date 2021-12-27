#include "connection.h"

Connection::Connection(int h_socket, StreamProcessor * stream_processor, ConnectionMode mode) {
    closed = false;
    this->mode = mode;
    this->h_socket = h_socket;
    this->stream_processor = stream_processor;

    if (mode == ConnectionMode::BLUETOOTH_SOCKET) {
        debug("Waiting for bluetooth connection\n");
    } else {
        debug("Waiting for network connection\n");
    }
}

void Connection::accept() {
    client = -1;

#ifdef BLUETOOTH
    if (mode == ConnectionMode::BLUETOOTH_SOCKET) {
        struct sockaddr_rc blue_rem_addr;
        memset(&blue_rem_addr, 0, sizeof(blue_rem_addr));
        socklen_t opt = sizeof(blue_rem_addr);
        client = ::accept(h_socket, (struct sockaddr *) &blue_rem_addr, &opt);
    }
#endif

    if (mode == ConnectionMode::NETWORK_SOCKET) {
        struct sockaddr_in net_rem_addr;
        memset(&net_rem_addr, 0, sizeof(net_rem_addr));
        socklen_t opt = sizeof(net_rem_addr);
        client = ::accept(h_socket, (struct sockaddr *) &net_rem_addr, &opt);
    }

    if (client != -1) {
        debug("Connection accepted: %d\n", client);

        pthread_t thread_handle;
        pthread_create(&thread_handle, NULL, pthread_callback, this);
    } else {
        perror("Connection not accepted");
        sleep(1);
    }
}

void * Connection::pthread_callback(void *arg) {
    Connection * connection = (Connection *)arg;
    connection->connection_thread_runner();
    return NULL;
}

void Connection::connection_thread_runner() {
    debug("Thread start!\n");

    while (1) {
        size_read = read(client, buff, sizeof(buff));
        if (size_read <= 0) {
            debug("data read size: %ld\n", size_read);
            break;
        }

        int result = stream_processor->process(this);
        if (result < 0) {
            debug("process finished\n");
            break;
        }
    }
    close(client);
    closed = true;

    debug("Thread done!\n");
}

