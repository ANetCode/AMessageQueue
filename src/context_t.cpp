#include "amq_priv.h"
#include <stdlib.h>
using namespace amq;

context_t::context_t() {
    this->ptr = nullptr;
}

void context_t::init() {
    context_impl_t* impl = new context_impl_t();
    this->ptr = impl;
}

context_t::~context_t() {
    delete ptr;
    ptr = nullptr;
}

void context_t::addPollItem(poll_t *p){
    ((context_impl_t*)ptr)->addPollItem(p);
}

void context_t::Poll() {
    if (!validate()) {
        return;
    }
    ((context_impl_t*)ptr)->Poll();
}

bool context_t::validate () {
    if (ptr == nullptr) {
        return false;
    }
    return true;
}

context_impl_t::context_impl_t() : context_t()
{
    loop = ev_loop_new();
    LOGD() << "loop       ===>: " << loop << LOGEND();
}

void context_impl_t::Poll() {
    ev_loop(loop, EVLOOP_NONBLOCK);
    for(auto p : poll_list) {
        p->Poll();
    }
}