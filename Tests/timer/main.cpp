#include <iostream>
using namespace std;
#include "amq.h"
using namespace amq;

int main(int argc, char const *argv[]) {
    context_t context;

    context.init();
    timer_t* timer = timer_t::create(&context, 5.0f);

     // 轮询context
     while(true) {
        context.Poll();
        usleep(500);
    }

    return 0;
}