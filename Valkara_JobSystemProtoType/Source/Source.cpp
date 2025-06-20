#include <cassert>
#include <iostream>

#include "JobSystemProtoType/JobSystemProtoType.h"
#include "ThreadSafeQueue/LockingThreadSafeQueue.h"
#include "ThreadPool/ThreadPool.h"

#define LOCKING_QUEUE_TEST 0
#define THREAD_POOL_TEST 0
#define JOB_SYSTEM_PROTOTYPE_TEST 1

#if THREAD_POOL_TEST
void TestThreadPoolBasic(ThreadPool& poolToTest, int TestAmount)
{
	ThreadPool& pool = poolToTest;

	std::atomic<int> counter{ 0 };

	std::mutex consoleMutex;

	// Enqueue several tasks
	for (int i = 0; i < TestAmount; ++i)
	{
		pool.Enqueue
		(
			[&counter, &consoleMutex] 
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 10 + 1));

				int value = ++counter;
				{
					std::lock_guard<std::mutex> lock(consoleMutex); // the window is a shared resources needs a mutex
					std::cout << "Test processing: " << value << " Task \n";
				}
			}
		);
	}

	// Wait for tasks to finish
	while (pool.Size() > 0)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}

	// Give threads a moment to finish incrementing
	std::this_thread::sleep_for(std::chrono::milliseconds(100));

	assert(counter == TestAmount);
	std::cout << "Test Thread Pool Basic Passed! ThreadPool Processed: " << TestAmount << " Tasks \n";
}
#endif

int main()
{
#if LOCKING_QUEUE_TEST
	LockingThreadSafeQueue<int> queue;

	queue.Push(42);
	queue.Push(41);
	queue.Push(2);
	queue.Push(4);

	auto val = queue.TryPop();

	if (val.has_value())
		std::cout << "Got: " << val.value() << "\n";


	val = queue.TryPop();

	if (val.has_value())
		std::cout << "Got: " << val.value() << "\n";


	val = queue.TryPop();

	if (val.has_value())
		std::cout << "Got: " << val.value() << "\n";


	val = queue.TryPop();

	if (val.has_value())
		std::cout << "Got: " << val.value() << "\n";
#endif

#if THREAD_POOL_TEST
	{
		ThreadPool testingPool(4);

		testingPool.Enqueue
		(
			[]
			{
				std::cout << "Hello from thread pool!\n"; 
			}
		);

		// Add a small delay
		while (testingPool.Size() > 0)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}

		TestThreadPoolBasic(testingPool, 150);
		TestThreadPoolBasic(testingPool, 1);
		TestThreadPoolBasic(testingPool, 0);
		TestThreadPoolBasic(testingPool, 2000);

	} 	// Testing Thread Pool Destructor
#endif

#if JOB_SYSTEM_PROTOTYPE_TEST
	// Testing Scope for JobSystema
	{
		JobSystemProtoType TestingJobSystem;
	}
#endif

	return 0;
}