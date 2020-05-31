//
// Created by imrip on 3/27/19.
//

#include <iostream>
#include "Scheduler.h"

Scheduler::~Scheduler() = default;

int Scheduler::get_running_thread() {
    return running;
}

int Scheduler::register_blocked_thread(int tid) {
    blocked.push_back(tid);
    if (tid == running) {
        scheduling_decision();
    }
    return 0;
}

int Scheduler::register_new_thread() {
    if (free_ids.empty()) return -1;
    int tid = free_ids.top();
    ready.push_back(tid);
    free_ids.pop();
    quantum_count[tid] = 0;
    return tid;
}


void end_quantum(int sig) {
    Scheduler::instance()->scheduling_decision();
}

int Scheduler::scheduling_decision() {
    num_quantums++;
#ifdef TEST_SCHEDULER
    printf("making schedule decision...\n");
#endif
    ready.push_back(running);
    do {
        running = ready.front();
        ready.pop_front();
    } while (running_is_blocked() || running_is_asleep());
    quantum_count[running] = quantum_count[running] + 1;
#ifdef TEST_SCHEDULER
    printf("now running: %d\n", running);
#endif
    return 0;
}

void Scheduler::print_free_ids() {
    priority_queue<int, std::vector<int>, greater<>> temp;
    while (!free_ids.empty()) {
        auto current = free_ids.top();
        std::cout << current << ", ";
        temp.push(current);
        free_ids.pop();
    }
    std::cout << std::endl;
    while (!temp.empty()) {
        auto current = temp.top();
        free_ids.push(current);
        temp.pop();
    }
}

int Scheduler::set_max_running_thread(unsigned int max_running_threads) {
    this->max_running_threads = max_running_threads;
    this->running = 0;
    for (int i = 1; i < max_running_threads; i++) {
        free_ids.push(i);
    }
#ifdef TEST_SCHEDULER
    print_free_ids();
#endif
    return 0;
}

Scheduler *Scheduler::instance() {
    if (!s_instance)
        s_instance = new Scheduler;
    return s_instance;
}

int Scheduler::register_sleeping_thread(int tid, int usecs) {
    return 0;
}

void Scheduler::print_blocked() {
    for (int &i : blocked) {
        std::cout << i << ',';
    }
    std::cout << std::endl;
}

bool Scheduler::empty() {
    return free_ids.size() == max_running_threads;
}

bool Scheduler::running_is_blocked() {
    auto idx = std::find(blocked.begin(), blocked.end(), running);
    return idx != blocked.end();
}

unsigned int Scheduler::get_total_quantums() {
    return num_quantums;
}

int Scheduler::start_timer(unsigned int u_sec) {
	struct itimerval timer{};
	struct sigaction sa = {nullptr};

	sa.sa_handler = &end_quantum;
	if (sigaction(SIGVTALRM, &sa, nullptr) < 0) {
		fprintf(stderr, "system error: sigaction error. \n");
	}

	// Configure the timer to expire after u_sec sec... */
	timer.it_value.tv_sec = 0;        // first time interval, seconds part
	timer.it_value.tv_usec = u_sec;        // first time interval, microseconds part

	// configure the timer to expire every u_suc after that.
	timer.it_interval.tv_sec = 0;    // following time intervals, seconds part
	timer.it_interval.tv_usec = u_sec;    // following time intervals, microseconds part

	// Start a virtual timer. It counts down whenever this process is executing.
	if (setitimer(ITIMER_VIRTUAL, &timer, nullptr)) {
		fprintf(stderr, "system error: setitimer error. \n");
		return -1;
	}
	return 0;
}

int Scheduler::unblock_thread(int tid) {
	auto idx = std::find(blocked.begin(), blocked.end(), tid);
	if (idx < blocked.end()) {
		blocked.erase(idx);

		//TODO - how can it be in ready state if it's in blocked state? this if is redundant...
		if (!is_ready(tid)) {
			ready.push_back(tid);
		}
		return 0;
	} else {
		return -1;
	}
}

bool Scheduler::is_ready(int tid) {
	return std::find(ready.begin(), ready.end(), tid) != ready.end();
}

bool Scheduler::running_is_asleep() {
	return false;
}

int Scheduler::get_num_of_personal_quantums(int tid) {
	auto search = quantum_count.find(tid);
	if (search == quantum_count.end()) {
		return -1;
	}
	else {
		return search->second;
	}
}

unsigned int Scheduler::getNumOfThreads()
{
	return (unsigned int)quantum_count.size();
}

void Scheduler::deleteReadyQueue()
{
	ready.clear();
}

void Scheduler::deleteFreeIdsQueue() {
    free_ids = priority_queue<int, std::vector<int>, greater<>>();
}

int Scheduler::deregister_thread(int tid) {
    auto ready_idx = std::find(ready.begin(), ready.end(), tid);
    if (ready_idx != ready.end()) {
        ready.erase(ready_idx);
    }
    auto blocked_idx = std::find(blocked.begin(), blocked.end(), tid);
    if (blocked_idx != blocked.end()) {
        blocked.erase(blocked_idx);
    }
    // TODO remove from sleeping as well
    if (ready_idx == ready.end() && blocked_idx==blocked.end()){
        return NO_SUCH_THREAD;
    }
    quantum_count.erase(tid);
    free_ids.push(tid);
    if (tid == running) {
        scheduling_decision();
        return RUNNING_THREAD_TERMINATED;
    }
    return 0;
}
