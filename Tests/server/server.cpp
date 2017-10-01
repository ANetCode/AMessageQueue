#include "common.h"
using namespace amq;
using namespace std;

static void OnMessage(amq::message_t msg) {
    printf("============================>%p, %s\n", &msg, msg.data());

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

void run_server(int argc, const char** argv) {
    amq::context_t context;
    amq::protocol_t *protocol;

    protocol = amq::protocol_t::Bind(&context, "tcpmsg://0.0.0.0:9981");
    if (protocol == nullptr) {
        cout << "protocol not support!" << endl;
        return ;
    }
    protocol->OnMessageCallback = OnMessage;
    while(true) {
        context.Poll();
        if (protocol->hasRequest()) {
            // amq::message_t request;
            // protocol->recv(&request);
        }

        // printf("poll one.\n");
        usleep(500);
    }
}