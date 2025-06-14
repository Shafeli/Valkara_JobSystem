#include <iostream>
#include <mutex>
#include <queue>
#include <condition_variable>
#include <optional>

template <typename Type>
class ThreadSafeQueue
{
	std::queue<Type> m_queue;
	mutable std::mutex m_mutex;
	std::condition_variable m_conditionVariable;

	void Push(const Type& value)
	{
		{ // scope to unlock mutex

			std::lock_guard<std::mutex> lock(m_mutex);
			m_queue.push(value);
			
		}
		m_conditionVariable.notify_one();
	}

	Type Pop()
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		m_conditionVariable.wait(lock, [this] {return !m_queue.empty(); }); //condition.wait(mutex)  atomically releases mutex and waits
		Type value = m_queue.front();
		m_queue.pop();
		return value;
	}

	std::optional<Type> TryPop()
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		if (m_queue.empty())
			return std::nullopt;

		Type value = m_queue.front();
		m_queue.pop();
		return value;
	}

	bool IsEmpty() const
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		return m_queue.empty();
	}

};

int main()
{



	return 0;
}