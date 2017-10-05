#include "amq_priv.h"
using namespace amq;

static void timeout_cb (struct ev_loop *loop, ev_timer *w, int revents);

class timer_impl_t : public amq::timer_t {
public:
    ev_timer timeout_watcher;
};

timer_t::timer_t() {
    OnTimeout = nullptr;
}

timer_t* timer_t::create(context_t* context, float duration) {
    timer_impl_t* ptr = new timer_impl_t();
    struct ev_loop *loop = context->GetImpl()->loop;
    ev_timer *timeout_watcher = &(ptr->timeout_watcher);

    timeout_watcher->data = ptr;
    ev_timer_init (timeout_watcher, timeout_cb, duration, 0.);
    ev_timer_start (loop, timeout_watcher);
    LOGD() << "setup a timer " << duration << "s";
    return ptr;
}

void timer_t::timeout() {
    if (OnTimeout != nullptr) {
        OnTimeout(this);
    }
}

static void
timeout_cb (struct ev_loop *loop, ev_timer *w, int revents) {
    timer_t *t = (timer_t *)w->data;
    if (t != nullptr) {
        t->timeout();
    }
    ev_break (EV_A_ EVBREAK_ONE);
    delete t;
}