#include "tcp_io.h"
#include "tcp_protocol.h"
using namespace amq;

int32_t MESSAGE_MAX_SIZE = 16 * 1024 * 1024;

tcp_io_t::tcp_io_t(context_t* context) {
    recving = false;
    this->context = context;
    msg.alloc(MESSAGE_MAX_SIZE);
}
void tcp_io_t::on_read() {
    int BUFFER_SIZE = 6;
    char buffer[BUFFER_SIZE];
    ssize_t read;
    
    if (isAlive == false) {
        return;
    }
    
    if(EV_ERROR & m_io.events) {
        LOGE() << ("got invalid event") << strerror(errno);;
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
        LOGD() << "read error. fd: " << fd << " reson: " << strerror(errno) << LOGEND();
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
    struct ev_loop *loop = context->GetImpl()->loop;
    ev_io_stop(loop, &m_io);
    close(fd);
    fd = -1;
    LOGE() << "peer might closing:" << strerror(errno);
    protocol->OnDisconnect();
    
    delete this;
}
