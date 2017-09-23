#include "tcp_protocol.h"
using namespace amq;

int32_t MESSAGE_MAX_SIZE = 6;
static int total_clients;
tcp_io_t::tcp_io_t() {
    recving = false;
    msg.alloc(16 * 1024 * 1024);
}
void tcp_io_t::on_read() {
    int BUFFER_SIZE = 6;
    char buffer[BUFFER_SIZE];
    ssize_t read;
    
    if (isAlive == false) {
        return;
    }
    
    if(EV_ERROR & m_io.events) {
        perror("got invalid event");
        return;
    }
    memset(buffer, 0, BUFFER_SIZE);
    // calc the size in this recv action
    int recv_size = BUFFER_SIZE;
    if (recving == true) {
        int left_bytes = msg_len - msg.size();
        if (left_bytes < BUFFER_SIZE) {
            recv_size = left_bytes;
        }
    }
    // Receive message from client socket
    read = ::recv(fd, buffer, recv_size, 0);
    
    if(read < 0) {
        perror("read error");
        return;
    }
    
    if(read == 0) {
        release();
        return;
    } else {
        if (recving == false) {
            msg_len = *(int32_t*) buffer;
            if (msg_len > MESSAGE_MAX_SIZE) {
                release();
                return;
            }
            recving = true;
            msg.reset();
            msg.alloc(msg_len);
            msg.copy((uint8_t*)(buffer+4), read - 4);
        } else {
            msg.copy((uint8_t*)(buffer), read);
            
            if (msg_len == msg.size()) {
                msg_len = 0;
                recving = false;
                protocol->OnMessage(this, &msg);
                // no more code
                // cause of maybe this class instance 
                // has been deleted, some where invoke release()
                return;
            }
        }
    }
}

void tcp_io_t::on_send(message_t* msg) {
    printf("on_send:[%s] %zu\n", msg->data(), msg->size());
    int32_t len = msg->size();
    if (len > MESSAGE_MAX_SIZE) {
        printf("send %p oom, len: %d\n", msg, len);
        this->release();
        return;
    }
    ::send(fd, &len, sizeof(len), 0);
    ::send(fd, msg->data(), msg->size(), 0);
}

void tcp_io_t::release() {
    // Stop and free watchet if client socket is closing
    isAlive = false;
    ev_io_stop(loop, &m_io);
    close(fd);
    fd = -1;
    perror("peer might closing");
    total_clients --; // Decrement total_clients count
    printf("%d client(%p) connected.\n", total_clients, this);
    delete this;
}
