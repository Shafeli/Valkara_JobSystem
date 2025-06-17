#include <cassert>
#include <iostream>
#include "ThreadSafeQueue/LockingThreadSafeQueue.h"
#include "ThreadPool/ThreadPool.h"

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
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
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

int main()
{

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


	ThreadPool testingPool(4);

	testingPool.Enqueue([] { std::cout << "Hello from thread pool!\n"; });

	// Add a small delay
	while (testingPool.Size() > 0)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

	TestThreadPoolBasic(testingPool, 150);
	TestThreadPoolBasic(testingPool, 1);
	TestThreadPoolBasic(testingPool, 0);
	TestThreadPoolBasic(testingPool, 2000);
	return 0;
}