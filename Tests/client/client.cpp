#include "common.h"
using namespace amq;
using namespace std;

void run_client(int argc, const char** argv) {
    LOGD() << "run client\n";
    amq::context_t context;
    amq::protocol_t *protocol;

    protocol = amq::protocol_t::InitConnector(&context, "tcpmsg://127.0.0.1:9981");
    if (protocol == nullptr) {
        cerr << "protocol error" << endl;
        return;
    }
    amq::message_t request;
    request.copy((const uint8_t*)"ping", 4);
    protocol->send(&request);
    sleep(1);
}