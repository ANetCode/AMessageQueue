#include "common.h"
using namespace amq;
using namespace std;

static void OnMessage(amq::message_t msg) {
    LOGD() << "============================>" << &msg << " data: " <<  msg.data() << LOGEND();

    if(strncmp("pong", (const char*)msg.data(), 4) == 0) {
        amq::message_t reply_msg;
        const char* content = "ping";
        LOGW() << "going to send ping again\n";
        sleep(3);
        reply_msg.copy((const uint8_t*)content, strlen(content));
        msg.reply(&reply_msg);
    }
}

static void OnDisconnect(protocol_t* p) {
    LOGE() << "protocol:" << p << " disconnected." << LOGEND();
    exit(0);
}

void run_client(int argc, const char** argv) {
    LOGD() << "run client\n";
    amq::context_t context;
    amq::protocol_t *protocol;
    
    context.init();
    // 创建连接器
    protocol = amq::protocol_t::InitConnector(&context, "tcpmsg://127.0.0.1:9981");
    if (protocol == nullptr) {
        LOGE() << "protocol error" << LOGEND();
        return;
    }
    // 注册回调
    protocol->OnMessageCallback = OnMessage;
    protocol->OnDisconnectCallback = OnDisconnect;
    // 给服务器发送一条消息
    amq::message_t request;
    request.copy((const uint8_t*)"ping", 4);
    protocol->send(&request);
    // 轮询context
    while(true) {
        context.Poll();
        usleep(500);
    }
}