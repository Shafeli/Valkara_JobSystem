#pragma once

#include <vector>
#include <thread>
#include <functional>
#include <atomic>
#include "../ThreadSafeQueue/LockingThreadSafeQueue.h"

class ThreadPool
{
	std::vector<std::thread> workers;
	LockingThreadSafeQueue<std::function<void()>> m_taskQueue;
	std::atomic<bool> m_run{ false };

public:
	ThreadPool(size_t threadCount)
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

	~ThreadPool()
	{
		m_run = false;
		for (auto& thread: workers)
		{
			if (thread.joinable())
				thread.join();
		}
	}

	size_t Size() { return m_taskQueue.Size(); }

	template<typename task>
	void Enqueue(task&& T)
	{
		m_taskQueue.Push(std::function<void()>(std::forward<task>(T)));
	}
};

