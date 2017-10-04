#ifndef AMQ_SOCKET_H
#define AMQ_SOCKET_H

#include "../amq_priv.h"
#include "socket_utils.h"
#include <list>
#include "../utils/utils.h"

namespace amq {
class tcp_protocol_t;
class tcp_io_t;

class tcp_protocol_t : public protocol_t {
public:
    tcp_protocol_t();

    bool Bind(std::string info);
    bool connect(std::string info);

    void OnAccept();
    void OnMessage(tcp_io_t* reader, message_t* msg);
    void send(message_t*);
    bool isAlive();
protected:
    int                  fd;
    struct ev_io         m_io;
    
    std::list<message_t> recv_msg;
    tcp_io_t            *remote; /// < only client
};

}

#endif // AMQ_SOCKET_H