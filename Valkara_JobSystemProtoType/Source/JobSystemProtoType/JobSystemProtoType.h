#pragma once
#include "../ThreadPool/ThreadPool.h"
#include "Job.h"



class JobSystemProtoType
{
	ThreadPool m_threadPool;
    std::list<std::shared_ptr<JobHandle>> m_activeWork;
    std::mutex m_activeWorkMutex;

public:
	JobSystemProtoType()
		:m_threadPool(4) // defaulting to 4 worker threads for now
	{

	}

    // Submits a function to the thread pool and returns a JobHandle.
    std::shared_ptr<JobHandle> Submit(std::function<void()> work)
    {
        auto counter = std::make_shared<std::atomic<int>>(1);
        auto meta = std::make_shared<JobMetaData>();
        meta->counter = counter.get();

        auto jobHandle = std::make_shared<JobHandle>();
        jobHandle->Metrics = meta;

        {
            std::lock_guard<std::mutex> lock(m_activeWorkMutex);
            m_activeWork.emplace_back(jobHandle); // store shared_ptr
        }

        std::weak_ptr<JobHandle> weakHandle = jobHandle; // weak capture for cleanup

        jobHandle->Task =
            [this, func = std::move(work), meta, weakHandle]()
            {
                meta->startTime = std::chrono::steady_clock::now(); // Start profile timer

                try
                {
                    func(); // Do the real work
                }
                catch (...)
                {
                    // TODO: log or handle
                    throw;
                }

                meta->endTime = std::chrono::steady_clock::now(); // End profile timer

                meta->counter->fetch_sub(1, std::memory_order_release); // More explicit and gives you memory ordering control

                if (auto strongHandle = weakHandle.lock()) 
                {
                    std::lock_guard<std::mutex> lock(this->m_activeWorkMutex);

                    this->m_activeWork.remove_if([&](const std::shared_ptr<JobHandle>& h) 
                        {
                        return h == strongHandle;
                        });
                }
            };

        m_threadPool.Enqueue(jobHandle->Task);
        return jobHandle;
    }

};

