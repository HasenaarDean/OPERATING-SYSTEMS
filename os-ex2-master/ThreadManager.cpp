//
// Created by imrip on 3/27/19.
//

#include "ThreadManager.h"
#define RUNNING_STATE 0
#define READY_STATE 1
#define BLOCKED_STATE 2


ThreadManager :: ThreadManager(void(*newFunc)(void), unsigned int newStateOfThread,
		                       char stackOfThread[STACK_SIZE], unsigned int newId) :

							 funcEntryPointOfThread(newFunc),
							 currentStateOfThread(newStateOfThread), idOfThread(newId)
{

	address_t sp, pc;

	sp = (address_t)stackOfThread + STACK_SIZE - sizeof(address_t);
	pc = (address_t)newFunc;

	sleepUsecsLeft = 0;
	numOfMyQuanta = 0;

	sigsetjmp(env, NONZERO);

	(env->__jmpbuf)[JB_SP] = translate_address(sp);
	(env->__jmpbuf)[JB_PC] = translate_address(pc);

	sigemptyset(&env->__saved_mask);

}

unsigned int ThreadManager :: getNumOfMyQuanta() const
{
	return numOfMyQuanta;
}

void ThreadManager :: setNumOfMyQuanta(unsigned int newQuanta)
{
	numOfMyQuanta = newQuanta;
}

void ThreadManager :: incrementNumOfMyQuanta()
{
	numOfMyQuanta++;
}


void ThreadManager :: setIdOfThread(unsigned int newId)
{
	idOfThread = newId;
}

unsigned int ThreadManager :: getIdOfThread() const
{
	return idOfThread;
}

void ThreadManager :: setStateOfThread(unsigned int newState)
{
	currentStateOfThread = newState;
}

unsigned int ThreadManager :: getStateOfThread() const
{
	return currentStateOfThread;
}

void ThreadManager :: setSleepUsecsLeft(unsigned int newSleepUsecsLeft)
{
	sleepUsecsLeft = newSleepUsecsLeft;
}

unsigned int ThreadManager :: getSleepUsecsLeft() const
{
	return sleepUsecsLeft;
}

