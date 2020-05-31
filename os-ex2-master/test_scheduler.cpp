//
// Created by imrip on 4/6/19.
//

#include <cstdio>

#include "Scheduler.h"

Scheduler *Scheduler::s_instance = nullptr;

int main() {
    printf("set_max_running_threads:\n");
    Scheduler::instance()->set_max_running_thread(10);

    printf("\nregister new threads:\n");
    for (int i = 0; i < 8; i++){
        auto tid = Scheduler::instance()->register_new_thread();
    }

    printf("\nstart!\n");
    Scheduler::instance()->start_timer(500000);

    while (Scheduler::instance()->get_total_quantums() < 8){}

    printf("\nblocking threads: 2,4,8\n");
    Scheduler::instance()->register_blocked_thread(2);
    Scheduler::instance()->register_blocked_thread(4);
    Scheduler::instance()->register_blocked_thread(8);

    while (Scheduler::instance()->get_total_quantums() < 16){}

    printf("\nthread 1 ran for %d quantums\n", Scheduler::instance()->get_num_of_personal_quantums(1));
    printf("\nthread 2 ran for %d quantums\n", Scheduler::instance()->get_num_of_personal_quantums(2));

    printf("\nunblocking thread: 1\n");
    Scheduler::instance()->unblock_thread(1);
    while (Scheduler::instance()->get_total_quantums() < 24){}

    printf("\nderegister threads: 1,2,3\n");
    Scheduler::instance()->deregister_thread(1);
    Scheduler::instance()->deregister_thread(2);
    Scheduler::instance()->deregister_thread(3);
    while (Scheduler::instance()->get_total_quantums() < 32){}


    printf("\nderegister threads: 4,5,6,7,8\n");
    Scheduler::instance()->deregister_thread(3);
    Scheduler::instance()->deregister_thread(5);
    Scheduler::instance()->deregister_thread(6);
    Scheduler::instance()->deregister_thread(7);
    Scheduler::instance()->deregister_thread(8);
    while (Scheduler::instance()->get_running_thread() != 0){}
    return 0;
}