#ifndef __ABSTRACT_CONNECTION_H__
#define __ABSTRACT_CONNECTION_H__


#include <string.h>
#include <unistd.h>
#include <cerrno>
#include <stdio.h>

class AbstractConnection {
public:
    bool closed;
    int h_socket;
    int client;
    ssize_t size_read;
    char buff[1000];
};

#endif
