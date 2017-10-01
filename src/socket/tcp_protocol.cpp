#include "tcp_protocol.h"
using namespace std;

void accept_cb(struct ev_loop *loop, struct ev_io *watcher, int revents);
void read_cb(struct ev_loop *loop, struct ev_io *watcher, int revents);

using namespace amq;

tcp_protocol_t::tcp_protocol_t() :
    protocol_t()
{
    bHasRequest = false;
    remote = nullptr;
}

bool GetHostPort(const std::string& info, std::string& host, int& port) {
    // "tcpmsg://"
    if (info.size() < 9) {
        return false;
    }
    std::string hostport = info.substr(9, -1);
    int idx = hostport.find(':');
    if (idx < 0) {
        return false;
    }
    std::string h = hostport.substr(0, idx);
    std::string p = hostport.substr(idx + 1, -1);

    const char* chost = h.c_str();
    int cport = atoi(p.c_str());
    if (chost == nullptr ||
        h.empty() ||
        port == 0 ) {
        return false;
    }
    host = std::string(chost);
    port = cport;
    return true;
}

bool tcp_protocol_t::Bind(std::string info) {
    std::string host;
    int port;
    if (GetHostPort(info, host, port) == false) {
        return false;
    }
    fd = SocketServer(host.c_str(), port);

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
    if (remote != nullptr) { // 作为客户端的protocol 
        remote->on_send(msg);
    } else {
        if (msg->__priv_data == nullptr){
            LOGD() << "tcp_io_t __priv_data null\n";
            return;
        }
        tcp_io_t* io = (tcp_io_t*)msg->__priv_data;
        io->on_send(msg);
    }
}

bool tcp_protocol_t::isAlive() {
    return false;
}

bool tcp_protocol_t::connect(std::string info) {
    std::string host;
    int port;
    if (GetHostPort(info, host, port) == false) { 
        LOGD() << "connect error. host: " << host << " port: " << port << LOGEND();
        return false;
    }
    int fd = SocketConnect(host.c_str(), port);
    if (fd == -1) {
        LOGD() << "socket connect error." << LOGEND();
        return -1;
    }
    loop = ev_loop_new();
    
    // new client protocol
    remote = new tcp_io_t();
    remote->fd   = fd;
    remote->loop = loop;
    remote->m_io.data = remote;
    remote->protocol = this;
    remote->isAlive  = true;
    
    ev_io_init(&remote->m_io, read_cb, fd, EV_READ);
    ev_io_start(loop, &remote->m_io);

    return true;
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
