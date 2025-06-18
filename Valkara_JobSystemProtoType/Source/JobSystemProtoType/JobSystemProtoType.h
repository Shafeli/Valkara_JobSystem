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

	void Submit(const Job& work)
	{
		// TODO: Implement job submission logic
	}
};

