#pragma once

// general headers
#include <thread>
#include <atomic>
#include <future>
#include <vector>

// SUPER useful parfor
// lifted from Van Nugteren

// parallel for loop implementation as template class.
// Code modified from Andy Thomason:
// http://www.andythomason.com/2016/08/21/c-multithreading-an-effective-parallel-for-loop/

// a parfor loop can be made like this
// parfor(10,20,true,[&](arma::uword idx, int cpu) {
//        printf("task %d running on cpu %d\n", idx, cpu);
//     }
// );

// get number of available cpus for
// parfor loops
// int parfor_num_cpus(){
// 	return std::thread::hardware_concurrency();
// }

// parallel-for template class
template <class F>
void parfor(arma::uword begin, arma::uword end, bool enable_parallel, F fn) {
	// make index
	std::atomic<arma::uword> idx;
	idx = begin;

	// fire asynchronous threads
	// don't run as threads when loop only consists of one walk
	if(enable_parallel == true && end - begin > 0) {
		// get number of cpu's
		int num_cpus = std::thread::hardware_concurrency();
		char* str = std::getenv("RAT_NUM_THREADS");
		if(str != NULL) num_cpus = std::stoi(std::string(str));

		// calculate actual number of threads
		const int num_futures = int(std::min(arma::uword(num_cpus), end - begin + 1));

		// get number of available CPU cores
		// int num_cpus = std::thread::hardware_concurrency();
		std::vector<std::future<void>> futures(num_futures);

		// walk over available cpus
		for(int cpu = 0; cpu != num_futures; cpu++) {
			futures[cpu] = std::async(std::launch::async, [cpu, &idx, end, &fn]() {
				// keep firing tasks untill run out of idx
				for(;;) {
					// atomic increment of index so that no two threads can do same task
					arma::uword i = idx++;

					// check if last index
					if(i >= end) break;

					// run loop content with i
					fn(i, cpu);
				}
			});
		}

		// make sure all threads are finished
		for(int cpu = 0; cpu != num_futures; cpu++) {
			futures[cpu].get();
		}
	}

	// just run as regular loop
	else {
		for(arma::uword i = begin; i < end; i++) {
			fn(i, 0);
		}
	}

}
