/*
	All threads are sequential. They add to a common variable -- sum -- in using atomic type. The output indicates the overall time consumed (unit: microsecond). The first column is when non-atomic type is used in the single thread situation. 
	
	Author: Wenjie ZHENG
	Date:	2014/8/11
*/

#include<iostream>
#include<thread>
#include<chrono>
#include<vector>
#include<atomic>

typedef std::chrono::high_resolution_clock Clock;
typedef std::chrono::microseconds Microseconds;
const int ITERATION = 10000;
const int N = 20;
const int NUM_CORES = 4;
const int EXTRA_THREADS[NUM_CORES] = {0,1,2,3};
std::atomic<long> sum;

void sum_atomic(){
	for(int i = 0; i < ITERATION; ++i){
		++sum;
	}
}

int main(){
	
	std::cout << "0\t";
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
		std::cout << ms.count() << '\t';
		
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