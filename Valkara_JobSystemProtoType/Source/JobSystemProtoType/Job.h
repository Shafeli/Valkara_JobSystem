#pragma once



struct Job
{
private:
	// profiler variables and other internal data can be added here

public:
	std::function<void()> Task; // The task to be executed by the job
};