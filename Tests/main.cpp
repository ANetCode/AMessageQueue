#include "common.h"
using namespace amq;
using namespace std;

void OnMessage(amq::message_t msg) {
    printf("============================>%p, %s\n", &msg, msg.data());

    if(strncmp("ping", (const char*)msg.data(), 4) == 0) {
        amq::message_t reply_msg; 
        reply_msg.copy((const uint8_t*)"pon1234", 8);
        msg.reply(&reply_msg);
    }

    if(strncmp("quit", (const char*)msg.data(), 4) == 0) {
        amq::message_t reply_msg;
        reply_msg.copy((const uint8_t*)"pongd", 5);
        // io->release();
    }
}

void test() {
    amq::context_t context;
    amq::protocol_t *protocol;
    
    protocol = amq::protocol_t::Bind(&context, "tcp://0.0.0.0:9981");
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

/**
 * @brief      { function_description }
 *
 * @param[in]  argc  The argc
 * @param      argv  The argv
 *
 * @return     { description_of_the_return_value }
 */
int main(int argc, char const *argv[]) {
    cout << "Hello Test Sample!" << endl;
    if (argc > 1) {
        std::string opcode = std::string(argv[1]);
        cout << "opcode: " << opcode << endl;
        if (StringEquals(opcode, "server")) {
            printf("is server\n");
            run_server(argc, argv);
        } else if (StringEquals(opcode, "client")) {
            printf("is client\n");
            run_client(argc, argv);
        }
        return 0;
    }
    return 0;
}