#ifndef AMQ_SOCKET_H
#define AMQ_SOCKET_H

#include "../AMessageQueue_priv.h"
#include "socket_utils.h"
#include <list>

namespace amq {
class tcp_protocol_t;
class tcp_io_t;

class tcp_io_t {
public:
    tcp_io_t();
    void on_read();
    void on_send(message_t*);
    void release();
    int             fd;
    struct ev_io    m_io;
    struct ev_loop *loop;
    size_t          msg_len;
    bool            recving;
    message_t       msg;
    tcp_protocol_t *protocol;
    bool            isAlive;
};

class tcp_protocol_t : public protocol_t {
public:
    tcp_protocol_t();

    bool Bind(std::string info);
    void Poll();

    void OnAccept();
    void OnMessage(tcp_io_t* reader, message_t* msg);
    void send(message_t*);
protected:
    int             fd;
    struct ev_io    m_io;
    struct ev_loop *loop;
    std::list<message_t> recv_msg;
};

}

#endif // AMQ_SOCKET_H