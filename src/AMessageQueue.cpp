#include "AMessageQueue_priv.h"
#include <stdlib.h>
using namespace amq;

context_t::context_t() {

}

void context_t::AddProtocol(protocol_t *p){
    protocols.push_back(p);
}

void context_t::Poll() {
    for(auto p : protocols) {
        p->Poll();
    }
}

bool StringStartsWith(const std::string content, const std::string prefix) {
    return content.substr(0, prefix.size()) == prefix;
}
protocol_t::protocol_t() {
    OnMessageCallback = nullptr;
}

protocol_t* protocol_t::CreateProtocol (amq::context_t* ctx, std::string uri) {
    if (StringStartsWith(uri, "tcpmsg://")) {
        protocol_t* p = new tcp_protocol_t();
        p->context = ctx;
        ctx->AddProtocol(p);
        return p;
    }
    LOGD() << "protocol error: " << uri << LOGEND();
    return nullptr;
}
protocol_t* protocol_t::Bind(context_t* ctx, std::string uri) {
    protocol_t* p = CreateProtocol(ctx, uri);
    if (p == nullptr) return nullptr;
    if(p->Bind(uri) == false) {
        delete p;
        return nullptr;
    }
    return p;
}

protocol_t* protocol_t::InitConnector(context_t* ctx, std::string uri) {
    protocol_t* p = CreateProtocol(ctx, uri);
    LOGD() << p << LOGEND();
    if (p == nullptr) return nullptr;
    if (p->connect(uri)) {
        return p;
    }
    delete p;
    return nullptr;
}

message_t::message_t() {
    init();
}

message_t::message_t(const message_t &obj) {
    init();
    copy(&obj);
    m_protocol  = obj.m_protocol;
    __priv_data = obj.__priv_data;
}

message_t::message_t(protocol_t *p) {
    init();
    m_protocol = p;
}

message_t::~message_t() {
    free(m_data);
    m_data = nullptr;
}

void message_t::init() {
    m_data      = nullptr;
    m_size      = 0;
    m_capacity  = 0;
    m_protocol  = nullptr;
    __priv_data = nullptr;
}

void message_t::copy(const uint8_t *data, size_t size) {
    if (m_capacity - m_size < size) {
        alloc(size);
    }
    if (m_capacity - m_size >= size) {
        memcpy(m_data + m_size, data, size);
        m_size += size;
    }
}

void message_t::copy(const message_t *other) {
    copy(other->m_data, other->m_size);
}

void message_t::alloc(size_t size) {
    void* ptr = realloc(m_data, size);
    if (ptr != NULL) {
        m_data = (uint8_t*)ptr;
        m_size = 0;
        m_capacity = size;
    }
}

void message_t::reset() {
    memset(m_data, 0, m_capacity);
    m_size = 0;
}

void message_t::reply(message_t* msg) {
    if (m_protocol == nullptr) {
        printf("message reply error. protocol not found.\n");
        return;
    }
    msg->__priv_data = this->__priv_data;
    m_protocol->send(msg);
}

void message_t::send(std::string uri) {

}

void message_t::set_protocol(protocol_t* p) {
    m_protocol = p;
}
