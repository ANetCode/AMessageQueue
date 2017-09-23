#ifndef SOCKET_UTILS_H
#define SOCKET_UTILS_H

#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <resolv.h>
#include <strings.h>
#include <netdb.h> //hostent
#include <arpa/inet.h>
#include <string.h>
#include <fcntl.h>
#include <ev.h> // libev
#include <netinet/in.h>
#include <sys/types.h>

bool SetSocketBlockingEnabled(int fd, bool blocking);
int SocketServer(const char* bindAddress, int port);
int SocketConnect(const char* host, int port);
#endif // SOCKET_UTILS_H