#include "uthreads.h"

using namespace std;


void exitProcess(bool exitSuccessfullyOrNot)
{
	if(exitSuccessfullyOrNot)
	{
		exit(0);
	}
	else
	{
		exit(EXIT_FAILURE);
	}
}

bool threadDoesNotExist(int tid)
{
	if(tid < ID_OF_MAIN_THREAD || tid >= MAX_THREAD_NUM)
	{
		return true;
	}

	return threadsList[tid] == nullptr;

}


void clearDataAndExit(bool exitSuccessfullyOrNot)
{
	//if we only have the main thread:
	if(Scheduler::instance()->getNumOfThreads() == 1)
	{
		deleteMainThread();

		exitProcess(exitSuccessfullyOrNot);
	}
	else
	{

		//delete all threads which are not the main thread:
		for(ThreadManager* tempThread : threadsList)
		{
			unsigned int tempID = 0;

			if(tempThread == nullptr || tempThread->getIdOfThread() == ID_OF_MAIN_THREAD)
			{
				continue;
			}

			tempID = tempThread->getIdOfThread();

			deleteThread(tempID);

		}

		deleteMainThread();
		exitProcess(exitSuccessfullyOrNot);
	}
}

void deleteMainThread()
{
	delete threadsList[ID_OF_MAIN_THREAD];
	threadsList[ID_OF_MAIN_THREAD] = nullptr;
	Scheduler::instance()->deleteFreeIdsQueue();
	Scheduler::instance()->deleteReadyQueue();
}

void deleteThread(unsigned int threadId)
{
	delete threadsList[threadId];

	threadsList[threadId] = nullptr;

}

int uthread_init(int quantum_usecs) 
{

}

int uthread_spawn(void (*f)(void)) 
{
	return ERROR_RETURN_VALUE;
}

int uthread_terminate(int tid)
{
	blockOrUnblockSignals(BLOCK);

	if(threadDoesNotExist(tid))
	{
		return ERROR_RETURN_VALUE;
	}

	if(tid == ID_OF_MAIN_THREAD)
	{
		clearDataAndExit(true);
	}

	Scheduler::instance()->deregister_thread(tid);

	deleteThread((unsigned int)tid);

	blockOrUnblockSignals(UNBLOCK);

	if(Scheduler::instance()->get_running_thread() == tid)
	{
		Scheduler::instance().
	}



}

int uthread_block(int tid) {
	// move thread to blocked array
	// if blocked thread is running, get next from ready
	return ERROR_RETURN_VALUE;
}

int uthread_resume(int tid) {
	// if thread is in blocked array move it to ready
	// if the id is free or does not exist return -1
	return ERROR_RETURN_VALUE;
}

int uthread_sleep(int usec) {
	// if tid == 0 return -1
	// else sleep thread for usec
	return ERROR_RETURN_VALUE;
}

int uthread_get_tid() { return ERROR_RETURN_VALUE; }

int uthread_get_total_quantums() { return ERROR_RETURN_VALUE; }

int uthread_get_quantums(int tid) { return ERROR_RETURN_VALUE; }

void checkReturnValue(int returnValue, const char messageError[MESSAGE_MAX_LENGTH])
{
	if(returnValue != 0)
	{
		fprintf(stderr, messageError);

		exit(EXIT_FAILURE);

	}
}

/**
 * This function blocks or unblocks all signals in signals set.
 * @param blockSignals - do we block (true) or unblock (false).
 */
void blockOrUnblockSignals(bool blockSignals)
{

	int blockOrUnblock = SIG_BLOCK;

	//if we want to unblock signals:
	if(!blockSignals)
	{
		blockOrUnblock = SIG_UNBLOCK;
	}

	int returnValue = sigprocmask(blockOrUnblock, &setOfSignalsToBlock, nullptr);

	if(blockSignals)
	{
		checkReturnValue(returnValue, BLOCK_SIGNALS_ERR_MSG);
	}
	else
	{
		checkReturnValue(returnValue, UNBLOCK_SIGNALS_ERR_MSG);
	}

}


