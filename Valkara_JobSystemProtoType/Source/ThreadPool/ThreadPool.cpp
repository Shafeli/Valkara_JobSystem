#include "ThreadPool.h"

ThreadPool::ThreadPool(size_t threadCount)
{
	m_run = true;
	for (size_t i = 0; i < threadCount; ++i)
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
