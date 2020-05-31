//
// Created by imrip on 3/27/19.
//

#ifndef OS_EX2_SCHEDULER_H
#define OS_EX2_SCHEDULER_H

#define NO_SUCH_THREAD -1
#define RUNNING_THREAD_TERMINATED 1

#include <vector>
#include <queue>
#include <sys/time.h>
#include <stdio.h>
#include <signal.h>
#include <algorithm>
#include <map>

using namespace std;

class Scheduler {
public:

    int set_max_running_thread(unsigned int max_running_threads);

    /*
     * this function is called at the run of every quantum or when running thread is blocked/terminated.
     */
    int register_sleeping_thread(int tid, int usecs);

    int scheduling_decision();

    /*
     * returns new thread id;
     */
    int register_new_thread();

    int deregister_thread(int tid);

    int register_blocked_thread(int tid);

    int get_running_thread();

    static Scheduler *instance();

    int start_timer(unsigned int u_sec);

    bool empty();

    unsigned int get_total_quantums();

    int get_num_of_personal_quantums(int tid);

    int unblock_thread(int tid);

    unsigned int getNumOfThreads();

    void deleteReadyQueue();

    void deleteFreeIdsQueue();

private:
    static Scheduler *s_instance;

    Scheduler() = default;;
    unsigned int max_running_threads = 0;
    priority_queue<int, std::vector<int>, greater<>> free_ids; // free thread ids (never used or terminated)
    deque<int> ready;
    vector<int> blocked;
    vector<int> sleeping;
    map<int, int> quantum_count;
    int running{};
    unsigned int num_quantums = 0;

    ~Scheduler();

    void print_free_ids();

    void print_blocked();


    bool running_is_blocked();

    bool is_ready(int tid);

    bool running_is_asleep();

//private:
//    static Scheduler *s_instance;
//
//    Scheduler() = default;;
//    unsigned int max_running_threads = 0;
//    priority_queue<int, std::vector<int>, greater<>> free_ids; // free thread ids (never used or terminated)
//    deque<int> ready;
//    vector<int> blocked;
//    vector<int> sleeping;
//    map<int, int> quantum_count;
//    int running{};
//    unsigned int num_quantums = 0;
//
//    ~Scheduler();
//
//    void print_free_ids();
//
//    void print_blocked();
//
//
//    bool running_is_blocked();
//
//    bool is_ready(int tid);
//
//    bool running_is_asleep();
};


#endif //OS_EX2_SCHEDULER_H

