#include "amq_priv.h"
using namespace amq;

protocol_t::protocol_t() {
    OnMessageCallback = nullptr;
    OnDisconnectCallback = nullptr;
    totalConnection = 0;
}

protocol_t* protocol_t::CreateProtocol (amq::context_t* ctx, std::string uri) {
    if (StringStartsWith(uri, "tcpmsg://")) {
        protocol_t* p = new tcp_protocol_t();
        p->context = ctx;
        return p;
    }
    LOGD() << "protocol error: " << uri << LOGEND();
    return nullptr;
}
protocol_t* protocol_t::Bind(amq::context_t* ctx, std::string uri) {
    if (!ctx->validate()) {
        LOGE() << "context not init" << LOGEND();
        return nullptr;
    }
    protocol_t* p = CreateProtocol(ctx, uri);
    if (p == nullptr) return nullptr;
    if(p->Bind(uri) == false) {
        delete p;
        return nullptr;
    }
    return p;
}

protocol_t* protocol_t::InitConnector(context_t* ctx, std::string uri) {
    if (!ctx->validate()) {
        LOGE() << "context not init" << LOGEND();
        return nullptr;
    }
    protocol_t* p = CreateProtocol(ctx, uri);
    if (p == nullptr) return nullptr;
    if (p->connect(uri)) {
        return p;
    }
    delete p;
    return nullptr;
}

void protocol_t::OnDisconnect() {
    totalConnection--;
    if (OnDisconnectCallback != nullptr) {
        OnDisconnectCallback(this);
    }
}