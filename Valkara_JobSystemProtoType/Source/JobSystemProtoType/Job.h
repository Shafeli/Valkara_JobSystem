#pragma once


struct JobMetaData
{
	// TODO: ID system for Jobs
	std::atomic<int>* counter = nullptr;
	std::chrono::steady_clock::time_point startTime;
	std::chrono::steady_clock::time_point endTime;
};

struct Job
{
	std::function<void()> Task; // The task to be executed by the job
	std::shared_ptr<JobMetaData> Metrics;
};

struct JobHandle
{
	std::shared_ptr<std::atomic<int>> Counter;
	std::shared_ptr<JobMetaData> Metrics;

	[[nodiscard]] std::chrono::duration<double, std::milli> Duration() const
	{
		return Metrics ? Metrics->endTime - Metrics->startTime : std::chrono::milliseconds(0);
	}

	// Checking if the task has completed
	[[nodiscard]] bool IsDone() const
	{
		return Counter && Counter->load(std::memory_order_acquire) == 0;
	}


	void BusyWait() const
	{
		while (Counter && Counter->load(std::memory_order_acquire) > 0) 
		{
			std::this_thread::yield();
		}
	}
};