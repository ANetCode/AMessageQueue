#ifndef AMESSAGE_QUEUE_PRIV_H
#define AMESSAGE_QUEUE_PRIV_H

#include "amq.h"
#include "socket/tcp_protocol.h"
#include "utils/utils.h"

namespace amq {
class context_impl_t : public context_t {
public:
    context_impl_t();
    std::list<poll_t*> poll_list;
    void Poll();
    void addPollItem(poll_t *p) {
        poll_list.push_back(p);
    }
public:
    struct ev_loop      *loop;
};
}

#endif // AMESSAGE_QUEUE_PRIV_H