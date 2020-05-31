#include <stdio.h>
#include <setjmp.h>
#include <signal.h>
#include <unistd.h>
#include <sys/time.h>
#include <setjmp.h>

#ifndef OS_EX2_MANAGER_H
#define OS_EX2_MANAGER_H

#define SECOND 1000000
#define RUNNING_STATE 0
#define READY_STATE 1
#define BLOCKED_STATE 2
#define NONZERO 5
#define STACK_SIZE 4096

#ifdef __x86_64__
/* code for 64 bit Intel arch */

typedef unsigned long address_t;
#define JB_SP 6
#define JB_PC 7

/* A translation is required when using an address of a variable.
   Use this as a black box in your code. */
address_t translate_address(address_t addr)
{
	address_t ret;
	asm volatile("xor    %%fs:0x30,%0\n"
				 "rol    $0x11,%0\n"
	: "=g" (ret)
	: "0" (addr));
	return ret;
}

#else
/* code for 32 bit Intel arch */

typedef unsigned int address_t;
#define JB_SP 4
#define JB_PC 5

/* A translation is required when using an address of a variable.
   Use this as a black box in your code. */
address_t translate_address(address_t addr)
{
    address_t ret;
    asm volatile("xor    %%gs:0x18,%0\n"
		"rol    $0x9,%0\n"
                 : "=g" (ret)
                 : "0" (addr));
    return ret;
}

#endif

typedef unsigned long address_t;



class ThreadManager{
public:
    int run(int tid);
    int block(int tid);
    int terminate(int tid);

	ThreadManager(void (*newFunc)(void), unsigned int newStateOfThread,
			      char stackOfThread[STACK_SIZE], unsigned int newId);

	void setNumOfMyQuanta(unsigned int newQuanta);
	unsigned int getNumOfMyQuanta() const;
	void incrementNumOfMyQuanta();
	void setIdOfThread(unsigned int newId);
	unsigned int getIdOfThread() const;
	void setStateOfThread(unsigned int newState);
	unsigned int getStateOfThread() const;
	void setSleepUsecsLeft(unsigned int newSleepUsecsLeft);
	unsigned int getSleepUsecsLeft() const;

private:

	sigjmp_buf env;
	unsigned int numOfMyQuanta;
	void (*funcEntryPointOfThread)(void);
	unsigned int currentStateOfThread;
	char stackOfThread[STACK_SIZE];
	unsigned int idOfThread;
	unsigned int sleepUsecsLeft;

};


#endif //OS_EX2_MANAGER_H
