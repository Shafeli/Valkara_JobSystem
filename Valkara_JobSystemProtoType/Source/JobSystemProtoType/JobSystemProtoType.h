#pragma once
#include "../ThreadPool/ThreadPool.h"
#include "Job.h"



class JobSystemProtoType
{
	ThreadPool m_threadPool;

public:
	JobSystemProtoType()
		:m_threadPool(4) // defaulting to 4 worker threads for now
	{

	}

	// Submits a function to the thread pool and returns a JobHandle.
	JobHandle Submit(std::function<void()> work)
	{
        auto counter = std::make_shared<std::atomic<int>>(1);
        auto meta = std::make_shared<JobMetaData>();
        meta->counter = counter.get();

        JobHandle jobHandle;

        jobHandle.Task = 
            [func = std::move(work), meta]()
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

           // --(*meta->counter); // notify completion
            meta->counter->fetch_sub(1, std::memory_order_release); // More explicit and gives you memory ordering control
		};

        jobHandle.Metrics = meta;

        m_threadPool.Enqueue(jobHandle.Task);
        return jobHandle;
	}
};

