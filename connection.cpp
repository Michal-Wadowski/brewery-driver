#include "connection.h"

Connection::Connection(int h_socket, StreamProcessor * stream_processor) {
    this->h_socket = h_socket;
    this->stream_processor = stream_processor;
#ifdef BLUETOOTH
    debug("Bluetooth connection\n");
#else
    debug("Standard connection\n");
#endif
}

void Connection::accept() {
#ifdef BLUETOOTH
    struct sockaddr_rc rem_addr;
#else
    struct sockaddr_in rem_addr;
#endif

    memset(&rem_addr, 0, sizeof(rem_addr));
    socklen_t opt = sizeof(rem_addr);

    client = ::accept(h_socket, (struct sockaddr *) &rem_addr, &opt);

    debug("Connection accepted: %d\n", client);

    pthread_t thread_handle;
    pthread_create(&thread_handle, NULL, pthread_callback, this);
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
            break;
        }

        int result = stream_processor->process(buff, size_read);
        if (result < 0) {
            break;
        }
    }
    close(client);

    debug("Thread done!\n");
}

