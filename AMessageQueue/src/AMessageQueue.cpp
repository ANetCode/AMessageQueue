#include "AMessageQueue_priv.h"
#include <stdlib.h>
using namespace amq;

#define BACKTRACE
#ifdef BACKTRACE
#include <iostream>
#include <execinfo.h>
#include <cxxabi.h>
#include <string>
#include <stdio.h>
#include <vector>
#include <algorithm>
#include <memory>
std::vector<std::string> debug::GetStackTrace() {
    const int size = 128;
    int callstack[size] = {};
    int  frames = backtrace((void**) callstack, size);
    char** strs = backtrace_symbols((void**) callstack, frames);
    std::vector<std::string> stackFrames;
    stackFrames.reserve(frames);

    for (int i = 0; i < frames; ++i) {
        char functionSymbol[1024] = {};
        char moduleName[1024] = {};
        int  offset = 0;
        char addr[48] = {};
        sscanf(strs[i], "%*s %s %s %s %*s %d", (char*)&moduleName, (char*)&addr, (char*)&functionSymbol, &offset);
        int   validCppName = 0;
        char* functionName = abi::__cxa_demangle(functionSymbol, NULL, 0, &validCppName); 
        char stackFrame[4096] = {};
        if (validCppName == 0) { // success
            sprintf(stackFrame, "(%s)\t0x%s — %s + %d",
                    moduleName, addr, functionName, offset);
        } else {
            sprintf(stackFrame, "(%s)\t0x%s — %s + %d",
                    moduleName, addr, functionName, offset);
        }

        if (functionName) {
            free(functionName);
        }
        std::string frameStr(stackFrame);
        stackFrames.push_back(frameStr);
    }
    free(strs);
    return stackFrames;
}
#else
std::vector<std::string> debug::GetStackTrace() {}
#endif

void debug::PrintfStackTrace() {
    std::vector<std::string> bt = GetStackTrace();
    for(int i = 0; i < bt.size(); i++) {
        printf("%s\n", bt[i].c_str());
    }
}

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
    if (StringStartsWith(uri, "tcp://")) {
        protocol_t* p = new tcp_protocol_t;
        p->context = ctx;
        ctx->AddProtocol(p);
        return p;
    }
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
    if (p == nullptr) return nullptr;
    ctx->AddProtocol(p);
    return p;
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