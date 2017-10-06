#include "amq_priv.h"
using namespace amq;

static void timeout_cb (struct ev_loop *loop, ev_timer *w, int revents);

class timer_impl_t : public amq::atimer_t {
public:
    ev_timer timeout_watcher;
};

amq::atimer_t::atimer_t() {
    OnTimeout = nullptr;
}

atimer_t* amq::atimer_t::create(context_t* context, float duration) {
    timer_impl_t* ptr = new timer_impl_t();
    struct ev_loop *loop = context->GetImpl()->loop;
    ev_timer *timeout_watcher = &(ptr->timeout_watcher);

    timeout_watcher->data = ptr;
    ev_timer_init (timeout_watcher, timeout_cb, duration, 0.);
    ev_timer_start (loop, timeout_watcher);
    LOGD() << "setup a timer " << duration << "s";
    return ptr;
}

void amq::atimer_t::timeout() {
    if (OnTimeout != nullptr) {
        OnTimeout(this);
    }
}

static void
timeout_cb (struct ev_loop *loop, ev_timer *w, int revents) {
    atimer_t *t = (atimer_t *)w->data;
    if (t != nullptr) {
        t->timeout();
    }
    ev_break (EV_A_ EVBREAK_ONE);
    delete t;
}