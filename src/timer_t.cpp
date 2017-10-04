#include "amq_priv.h"
using namespace amq;

static void timeout_cb (struct ev_loop *loop, ev_timer *w, int revents);

class timer_impl_t : public amq::timer_t {
public:
    ev_timer timeout_watcher;
    struct ev_loop      *loop;
};

timer_t::timer_t() {

}

timer_t* timer_t::create(context_t* context, float duration) {
    timer_impl_t* ptr = new timer_impl_t();
    ev_timer_init (&ptr->timeout_watcher, timeout_cb, duration, 0.);
    ev_timer_start (ptr->loop, &ptr->timeout_watcher);
    return ptr;
}

void timer_t::OnTimeout() {
    LOGD() << "timeout";
}

static void
timeout_cb (struct ev_loop *loop, ev_timer *w, int revents) {
    timer_t *t = (timer_t *)w->data;
    if (t != nullptr) {
        t->OnTimeout();
    }
    ev_break (EV_A_ EVBREAK_ONE);
}