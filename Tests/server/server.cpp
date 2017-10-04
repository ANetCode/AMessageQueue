#include "common.h"
using namespace amq;
using namespace std;

static void OnMessage(amq::message_t msg) {
    LOGD() << "============================>" << &msg << " data: " <<  msg.data() << LOGEND();

    if(strncmp("ping", (const char*)msg.data(), 4) == 0) {
        amq::message_t reply_msg;
        const char* content = "pong1234567890ABCDEF";
        reply_msg.copy((const uint8_t*)content, strlen(content));
        msg.reply(&reply_msg);
    }

    if(strncmp("quit", (const char*)msg.data(), 4) == 0) {
        amq::message_t reply_msg;
        reply_msg.copy((const uint8_t*)"pong", 4);
        // io->release();
    }
}

static void OnNewConnect(protocol_t* p) {
    LOGW() << "protocol new connect:" << p << LOGEND();
    LOGW() << "protocol TotalConnections:" << p->TotalConnections();
}

static void OnDisconnect(protocol_t* p) {
    LOGE() << "protocol:" << p << " disconnected." << LOGEND();
}


void run_server(int argc, const char** argv) {
    amq::context_t context;
    amq::protocol_t *protocol;

    context.init();
    protocol = amq::protocol_t::Bind(&context, "tcpmsg://0.0.0.0:9981");
    if (protocol == nullptr) {
        LOGE() << "protocol not support!";
        return ;
    }
    protocol->OnMessageCallback = OnMessage;
    protocol->OnConnectedCallback = OnNewConnect;
    protocol->OnDisconnectCallback = OnDisconnect;
    while(true) {
        context.Poll();
        usleep(50000);
    }
}