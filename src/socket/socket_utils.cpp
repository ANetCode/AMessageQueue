#include "socket_utils.h"

bool SetSocketBlockingEnabled(int fd, bool blocking) {
    if (fd < 0)
        return false;
#ifdef WIN32
    unsigned long mode = blocking ? 0 : 1;
    return (ioctlsocket(fd, FIONBIO, &mode) == 0) ? true : false;
#else
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags < 0)
        return false;
    flags = blocking ? (flags&~O_NONBLOCK) : (flags|O_NONBLOCK);
    return (fcntl(fd, F_SETFL, flags) == 0) ? true : false;
#endif
}

int SocketServer(const char* bindAddress, int port) {
    int fd = -1;
    struct sockaddr_in addr;

    fd = socket (AF_INET, SOCK_STREAM, 0);
    // non block
    SetSocketBlockingEnabled(fd, false);
    // reuse port
    int enable = 1;
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
        perror("setsockopt(SO_REUSEADDR) failed");

    // init socket
    bzero (&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;

    // bind
    bind (fd, (struct sockaddr *)(&addr), sizeof(addr));

    // listen
    listen(fd, 10);
    return fd;
}
int SocketConnect(const char* host, int port) {
    int fd = -1;
    struct sockaddr_in addr;

    fd = socket (AF_INET, SOCK_STREAM, 0);
    
    
    // init socket
    bzero (&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;
 
    if (::connect(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) 
        perror("ERROR connecting");
    // non block
    SetSocketBlockingEnabled(fd, false);
    // reuse port
    int enable = 1;
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
        perror("setsockopt(SO_REUSEADDR) failed");

    return fd;
}