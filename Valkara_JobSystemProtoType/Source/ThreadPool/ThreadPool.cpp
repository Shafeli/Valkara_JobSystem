#include "ThreadPool.h"

#include <algorithm>

ThreadPool::ThreadPool(size_t threadCount)
{
	size_t hw_threads = std::thread::hardware_concurrency();
	size_t max_threads = std::max<size_t>(1, hw_threads - 1); // leave one for main/UI

	// Clamp and assign the sanitized thread count
	size_t threadAdjustment = std::clamp(threadCount, static_cast<size_t>(1), max_threads);

	m_run = true;
	for (size_t i = 0; i < threadAdjustment; ++i)
	{
		workers.emplace_back(

			[this]
			{
				while (m_run)
				{
					auto task = m_taskQueue.TryPop();
					if (task.has_value())
					{
						task.value()();
					}
					else
					{
						std::this_thread::yield();
					}
				}

			}

		);
	}
}

ThreadPool::~ThreadPool()
{
	m_run = false;
	for (auto& thread : workers)
	{
		if (thread.joinable())
			thread.join();
	}
}

void ThreadPool::Enqueue(std::function<void()> task)
{
	m_taskQueue.Push(std::move(task));
}
