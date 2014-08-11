/*
	All threads are parallel. They just do loops. The output indicates the overall time consumed (unit: microsecond). 
	
	Author: Wenjie ZHENG
	Date:	2014/8/11
*/

#include<iostream>
#include<thread>
#include<chrono>
#include<vector>

typedef std::chrono::high_resolution_clock Clock;
typedef std::chrono::microseconds Microseconds;
const int ITERATION = 10000000;
const int N = 20;
const int NUM_CORES = 4;
const int EXTRA_THREADS[NUM_CORES] = {0,1,2,3};

void iteration(){
	for(int i = 0; i < ITERATION; ++i);
}

int main(){
	
	for(int p = 0; p < NUM_CORES; ++p){
		std::cout << EXTRA_THREADS[p] + 1 << '\t';
	}
	std::cout << std::endl;
	
	for(int n = 0; n < N; ++n){
		for(int p = 0; p < NUM_CORES; ++p){
			std::vector<std::thread> threads;
			Clock::time_point t0 = Clock::now();
	
			for(int i = 0; i < EXTRA_THREADS[p]; ++i){
				threads.push_back(std::thread(iteration));
			}
			iteration();
			for(auto& thread : threads){
				thread.join();
			}

			Clock::time_point t1 = Clock::now();
			Microseconds ms = std::chrono::duration_cast<Microseconds>(t1 - t0);
			std::cout << ms.count() << '\t';
		}
		std::cout << std::endl;
	}
    return 0;
}