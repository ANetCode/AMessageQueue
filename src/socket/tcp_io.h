#ifndef AMQ_TCP_IO_H
#define AMQ_TCP_IO_H

#include "../amq_priv.h"
#include "socket_utils.h"
#include <list>
#include "../utils/utils.h"

namespace amq {
class tcp_protocol_t;
class tcp_io_t;    
class tcp_io_t {
public:
    tcp_io_t(context_t*);
    void on_read();
    void on_send(message_t*);
    void release();
    int             fd;
    struct ev_io    m_io;
    // struct ev_loop *loop;
    context_t      *context;
    size_t          msg_len;
    bool            recving;
    message_t       msg;
    tcp_protocol_t *protocol;
    bool            isAlive;
};
}

#endif //AMQ_TCP_IO_H