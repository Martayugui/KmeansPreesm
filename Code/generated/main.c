/**
 * @file main.c
 * @generated by CPrinter
 * @date Fri Jul 26 15:59:14 CEST 2019
 *
 */
// no monitoring by default
#define _GNU_SOURCE
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#ifdef __APPLE__
#include "TargetConditionals.h"
#endif

#include <pthread.h>
#include <stdio.h>

#define _PREESM_NBTHREADS_ 1
#define _PREESM_MAIN_THREAD_ 0

// application dependent includes
#include "preesm_gen.h"

// Declare computation thread functions
void *computationThread_Core0(void *arg);

pthread_barrier_t iter_barrier;
int stopThreads;


unsigned int launch(unsigned int core_id, pthread_t * thread, void *(*start_routine) (void *)) {

#ifdef _WIN32
	SYSTEM_INFO sysinfo;
	GetSystemInfo(&sysinfo);
	unsigned int numCPU = sysinfo.dwNumberOfProcessors;
#else
	unsigned int numCPU = sysconf(_SC_NPROCESSORS_ONLN);
#endif

	// init pthread attributes
	pthread_attr_t attr;
	pthread_attr_init(&attr);

	// check CPU id is valid
	if (core_id >= numCPU) {
		// leave attribute uninitialized
		printf("** Warning: thread %d will not be set with specific core affinity \n   due to the lack of available dedicated cores.\n",core_id);
	} else {
#ifdef __APPLE__
		// NOT SUPPORTED
#else
		// init cpuset struct
		cpu_set_t cpuset;
		CPU_ZERO(&cpuset);
		CPU_SET(core_id, &cpuset);

		// set pthread affinity
		pthread_attr_setaffinity_np(&attr, sizeof(cpuset), &cpuset);
#endif
	}

	// create thread
	pthread_create(thread, &attr, start_routine, NULL);
	return 0;
}


int main(void) {
	#ifdef _PREESM_MONITOR_INIT
	mkdir("papify-output", 0777);
	event_init_multiplex();
	#endif
	// Declaring thread pointers
	pthread_t coreThreads[_PREESM_NBTHREADS_];
	void *(*coreThreadComputations[_PREESM_NBTHREADS_])(void *) = {
		&computationThread_Core0
	};

#ifdef VERBOSE
	printf("Launched main\n");
#endif

	// Creating a synchronization barrier
	stopThreads = 0;
	pthread_barrier_init(&iter_barrier, NULL, _PREESM_NBTHREADS_);

	communicationInit();

	// Creating threads
	for (int i = 0; i < _PREESM_NBTHREADS_; i++) {
		if (i != _PREESM_MAIN_THREAD_) {
			if(launch(i,&coreThreads[i],coreThreadComputations[i])) {
				printf("Error: could not launch thread %d\n",i);
				return 1;
			}
		}
	}

	// run main operator code in this thread
	coreThreadComputations[_PREESM_MAIN_THREAD_](NULL);

	// Waiting for thread terminations
	for (int i = 0; i < _PREESM_NBTHREADS_; i++) {
		if (i != _PREESM_MAIN_THREAD_) {
			pthread_join(coreThreads[i], NULL);
		}
	}
	#ifdef _PREESM_MONITOR_INIT
	event_destroy();
	#endif

	return 0;
}

