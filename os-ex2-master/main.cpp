#include <iostream>
#include <stdio.h>
#include "uthreads.h"

void adder_thread() {
    int tid = uthread_get_tid();
    printf("adding from thread with tid: %d\n", tid);
    int res = 1+1;
    printf("whats 1 + 1? %d", res);
    uthread_terminate(tid);
}

void init(){
    uthread_init(100);
}

int main() {
    init();
    uthread_spawn(adder_thread);
    return 0;
}