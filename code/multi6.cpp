/*
	All threads are sequential. There are two parts in this program. 
	The first one adds to a common variable -- s1 -- in using mutex.
	The second one adds to another common variable -- s2 -- in using atomic type.
	In each iteration, a few (equal to NUM_TASKS) sub-additions are done.  
	The output indicates the overall time consumed (unit: microsecond). 
	
	Author: Wenjie ZHENG
	Date:	2014/8/11
*/

#include<iostream>
#include<thread>
#include<chrono>
#include<vector>
#include<mutex>
#include<atomic>

typedef std::chrono::high_resolution_clock Clock;
typedef std::chrono::microseconds Microseconds;
const int ITERATION = 1000;
const int NUM_TASKS = 100;
const int N = 20;
const int NUM_CORES = 4;
const int EXTRA_THREADS[NUM_CORES] = {0,1,2,3};
std::mutex mutex;
long s1 = 0;
std::atomic<long> s2;

void sum_mutex(){
	for(int i = 0; i < ITERATION; ++i){
		mutex.lock();
			for(int j = 0; j < NUM_TASKS; ++j){
				++s1;
			}
		mutex.unlock();
	}
}

void sum_atomic(){
	for(int i = 0; i < ITERATION; ++i){
		for(int j = 0; j < NUM_TASKS; ++j){
			++s2;
		}
	}
}

int main(){
	//mutex
	std::cout << "mutex" << std::endl;
	for(int p = 0; p < NUM_CORES; ++p){
		std::cout << EXTRA_THREADS[p] + 1 << '\t';
	}
	std::cout << std::endl;
	
	for(int n = 0; n < N; ++n){
		for(int p = 0; p < NUM_CORES; ++p){
			std::vector<std::thread> threads;
			s1 = 0;
			Clock::time_point t0 = Clock::now();
	
			for(int i = 0; i < EXTRA_THREADS[p]; ++i){
				threads.push_back(std::thread(sum_mutex));
			}
			sum_mutex();
			for(auto& thread : threads){
				thread.join();
			}

			Clock::time_point t1 = Clock::now();
			Microseconds ms = std::chrono::duration_cast<Microseconds>(t1 - t0);
			std::cout << ms.count() << '\t';
		}
		std::cout << std::endl;
	}
	
	//atomic
	std::cout << "atomic type" << std::endl;
	for(int p = 0; p < NUM_CORES; ++p){
		std::cout << EXTRA_THREADS[p] + 1 << '\t';
	}
	std::cout << std::endl;
	
	for(int n = 0; n < N; ++n){
		long s = 0;
		Clock::time_point t0 = Clock::now();
		for(int i = 0; i < ITERATION; ++i){
			++s;
		}
		Clock::time_point t1 = Clock::now();
		Microseconds ms = std::chrono::duration_cast<Microseconds>(t1 - t0);
//		std::cout << ms.count() << '\t';
		
		for(int p = 0; p < NUM_CORES; ++p){
			std::vector<std::thread> threads;
			t0 = Clock::now();
	
			for(int i = 0; i < EXTRA_THREADS[p]; ++i){
				threads.push_back(std::thread(sum_atomic));
			}
			sum_atomic();
			for(auto& thread : threads){
				thread.join();
			}

			t1 = Clock::now();
			ms = std::chrono::duration_cast<Microseconds>(t1 - t0);
			std::cout << ms.count() << '\t';
		}
		
		std::cout << std::endl;
	}
    return 0;
}