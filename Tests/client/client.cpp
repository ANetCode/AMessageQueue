#include "common.h"
using namespace amq;
using namespace std;

void run_client(int argc, const char** argv) {
    amq::context_t context;
    amq::protocol_t *protocol;
    printf("run client.\n");
    protocol = amq::protocol_t::InitConnector(&context, "tcp://0.0.0.0:9981");
    amq::message_t request;
    request.copy((const uint8_t*)"ping", 4);
    protocol->send(&request);
}