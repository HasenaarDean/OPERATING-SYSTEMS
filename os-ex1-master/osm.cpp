#include "osm.h"

#include <sys/time.h>
#include <cstdlib>

#define NUM_OF_COMMANDS_PER_ITERATION 10
#define SUCCESS 0
#define CONVERT_MICRO_SECS_TO_NANO_SECS 1000


struct timeval timeval_a, timeval_b;

/* Initialization function that the user must call
 * before running any other library function.
 * The function may, for example, allocate memory or
 * create/open files.
 * Pay attention: this function may be empty for some desings. It's fine.
 * Returns 0 upon success and -1 on failure
 */
int osm_init()
{
	return 0;
}

/* finalizer function that the user must call
 * after running any other library function.
 * The function may, for example, free memory or
 * close/delete files.
 * Returns 0 upon success and -1 on failure
 */
int osm_finalizer()
{
	return 0;
}

/* Time measurement function for a simple arithmetic operation.
   returns time in nano-seconds upon success,
   and -1 upon failure.
   */
double osm_operation_time(unsigned int iterations)
{
	if(iterations == 0)
	{
		iterations = 1000;
	}

	//round up number of iterations
    unsigned int rounded = iterations / NUM_OF_COMMANDS_PER_ITERATION;

	double beginningOfTimeInMicroSecs;
	double stopMeasureTimeInMicroSecs;
	double timeMeasuredInMicroSecs;

	//if there was a failure
	if(gettimeofday(&timeval_a, nullptr) != SUCCESS)
	{
		return -1;
	}
	else
	{
		unsigned int j;
		int result = 0;
		beginningOfTimeInMicroSecs = timeval_a.tv_usec;
		//loop unrolling
		for(j = 0 ; j < rounded ; j += NUM_OF_COMMANDS_PER_ITERATION)
		{
			result += 1;
			result += 1;
			result += 1;
			result += 1;
			result += 1;

			result += 1;
			result += 1;
			result += 1;
			result += 1;
			result += 1;
		}

		if(gettimeofday(&timeval_b, nullptr) != SUCCESS)
		{
			return -1;
		}
		else
		{

			stopMeasureTimeInMicroSecs = timeval_b.tv_usec;

			timeMeasuredInMicroSecs = stopMeasureTimeInMicroSecs - beginningOfTimeInMicroSecs;

			double convertMicroToNano = CONVERT_MICRO_SECS_TO_NANO_SECS * timeMeasuredInMicroSecs;

			return convertMicroToNano / rounded;

		}

	}
}

void foo() {}

/* Time measurement function for an empty function call.
   returns time in nano-seconds upon success,
   and -1 upon failure.
   */
double osm_function_time(unsigned int iterations)
{
	if(iterations == 0)
	{
		iterations = 1000;
	}

	//round up number of iterations
	unsigned int rounded = iterations / NUM_OF_COMMANDS_PER_ITERATION;

	double beginningOfTimeInMicroSecs;
	double stopMeasureTimeInMicroSecs;
	double timeMeasuredInMicroSecs;

	//if there was a failure
	if(gettimeofday(&timeval_a, nullptr) != SUCCESS)
	{
		return -1;
	}
	else
	{
		unsigned int j;
		beginningOfTimeInMicroSecs = timeval_a.tv_usec;
		//loop unrolling
		for(j = 0 ; j < rounded ; j += NUM_OF_COMMANDS_PER_ITERATION)
		{
			foo();
			foo();
			foo();
			foo();
			foo();

			foo();
			foo();
			foo();
			foo();
			foo();
		}

		if(gettimeofday(&timeval_b, nullptr) != SUCCESS)
		{
			return -1;
		}
		else
		{

			stopMeasureTimeInMicroSecs = timeval_b.tv_usec;

			timeMeasuredInMicroSecs = stopMeasureTimeInMicroSecs - beginningOfTimeInMicroSecs;

			double convertMicroToNano = CONVERT_MICRO_SECS_TO_NANO_SECS * timeMeasuredInMicroSecs;

			return convertMicroToNano / rounded;

		}

	}
}

/* Time measurement function for an empty trap into the operating system.
   returns time in nano-seconds upon success,
   and -1 upon failure.
   */
double osm_syscall_time(unsigned int iterations)
{
	if(iterations == 0)
	{
		iterations = 1000;
	}

	//round up number of iterations
    unsigned int rounded = iterations / NUM_OF_COMMANDS_PER_ITERATION;

	double beginningOfTimeInMicroSecs;
	double stopMeasureTimeInMicroSecs;
	double timeMeasuredInMicroSecs;

	//if there was a failure
	if(gettimeofday(&timeval_a, nullptr) != SUCCESS)
	{
		return -1;
	}
	else
	{
		unsigned int j;
		beginningOfTimeInMicroSecs = timeval_a.tv_usec;

		//loop unrolling
		for(j = 0 ; j < rounded ; j += NUM_OF_COMMANDS_PER_ITERATION)
		{
			OSM_NULLSYSCALL;
			OSM_NULLSYSCALL;
			OSM_NULLSYSCALL;
			OSM_NULLSYSCALL;
			OSM_NULLSYSCALL;

			OSM_NULLSYSCALL;
			OSM_NULLSYSCALL;
			OSM_NULLSYSCALL;
			OSM_NULLSYSCALL;
			OSM_NULLSYSCALL;
		}

		if(gettimeofday(&timeval_b, nullptr) != SUCCESS)
		{
			return -1;
		}
		else
		{

			stopMeasureTimeInMicroSecs = timeval_b.tv_usec;

			timeMeasuredInMicroSecs = stopMeasureTimeInMicroSecs - beginningOfTimeInMicroSecs;

			double convertMicroToNano = CONVERT_MICRO_SECS_TO_NANO_SECS * timeMeasuredInMicroSecs;

			return convertMicroToNano / rounded;

		}

	}
}