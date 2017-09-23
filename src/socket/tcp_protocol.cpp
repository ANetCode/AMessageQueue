#include "tcp_protocol.h"

void accept_cb(struct ev_loop *loop, struct ev_io *watcher, int revents);
void read_cb(struct ev_loop *loop, struct ev_io *watcher, int revents);

using namespace amq;

tcp_protocol_t::tcp_protocol_t() :
    protocol_t()
{
    bHasRequest = false;
}

bool tcp_protocol_t::Bind(std::string info) {
    if (info.size() < 6) {
        return false;
    }
    std::string hostport = info.substr(6, -1);
    int idx = hostport.find(':');
    if (idx < 0) {
        return false;
    }
    std::string h = hostport.substr(0, idx);
    std::string p = hostport.substr(idx + 1, -1);

    const char* host = h.c_str();
    int port = atoi(p.c_str());
    if (host == nullptr ||
        h.empty() ||
        port == 0 ) {
        return false;
    }
    fd = SocketServer(host, port);

    // init ev
    loop = ev_loop_new();
    ev_io_init(&m_io, accept_cb, fd, EV_READ);
    m_io.data = this;
    ev_io_start(loop, &m_io);
    return true;
}

void tcp_protocol_t::Poll() {
    ev_loop(loop, EVLOOP_NONBLOCK);
}

void tcp_protocol_t::OnMessage(tcp_io_t* io, message_t* msg) {
    if (OnMessageCallback != nullptr) {
        msg->set_protocol(this);
        msg->__priv_data = io;
        OnMessageCallback(*msg);
    }
}

void tcp_protocol_t::OnAccept () {
    struct sockaddr_in   client_addr;
    socklen_t            len;
    int                  client_sd;
    struct ev_io        *w_client;
    int                  revents;

    if(EV_ERROR & m_io.events) {
      perror("got invalid event");
      return;
    }
    // Accept client request
    client_sd = accept(m_io.fd, (struct sockaddr *)&client_addr, &len);

    if (client_sd < 0) {
        perror("accept error.");
        return;
    }

    // new client protocol
    tcp_io_t* io = new tcp_io_t();
    io->fd   = client_sd;
    io->loop = loop;
    io->m_io.data = io;
    io->protocol = this;
    io->isAlive  = true;

    ev_io_init(&io->m_io, read_cb, client_sd, EV_READ);
    ev_io_start(loop, &io->m_io);
}

void tcp_protocol_t::send(message_t*msg) {
    if (msg->__priv_data == nullptr){

        return;
    }
    tcp_io_t* io = (tcp_io_t*)msg->__priv_data;
    io->on_send(msg);
}

void accept_cb(struct ev_loop *loop, struct ev_io *watcher, int revents) {
    tcp_protocol_t* p = (tcp_protocol_t*)watcher->data;
    if (p != NULL) {
        p->OnAccept();
    }
}

void read_cb(struct ev_loop *loop, struct ev_io *watcher, int revents) {
    tcp_io_t* p = (tcp_io_t*)watcher->data;
    if (p != NULL) {
        p->on_read();
    }
}
