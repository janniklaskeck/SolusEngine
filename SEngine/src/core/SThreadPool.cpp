#include "core/SThreadPool.hpp"

namespace Solus
{
	SThreadPool::~SThreadPool()
	{
		{
			// Lock the queue to update the stop flag safely
			std::unique_lock<std::mutex> Lock(TaskQueueMutex);
			bStopQueue = true;
		}

		TaskQueueStateCondition.notify_all();

		for (auto& thread : Threads)
		{
			thread.join();
		}
	}

	void SThreadPool::InitThreads(const uint8 ThreadAmount)
	{
		if (!Threads.empty())
		{
			return;
		}

		for (uint8 i = 0; i < ThreadAmount; ++i)
		{
			Threads.emplace_back(&SThreadPool::ThreadLoop, this);
		}
	}

	void SThreadPool::EnqueueTask(std::function<void()> Task)
	{
		{
			std::unique_lock<std::mutex> Lock(TaskQueueMutex);
			TaskQueue.emplace(move(Task));
		}
		TaskQueueStateCondition.notify_one();
	}

	void SThreadPool::ThreadLoop()
	{
		while (true) 
		{
			std::function<void()> Task;
			// The reason for putting the below code
			// here is to unlock the queue before
			// executing the task so that other
			// threads can perform enqueue tasks
			{
				// Locking the queue so that data
				// can be shared safely
				std::unique_lock<std::mutex> Lock(TaskQueueMutex);

				// Waiting until there is a task to
				// execute or the pool is stopped
				TaskQueueStateCondition.wait(Lock, [this] 
					{
						return !TaskQueue.empty() || bStopQueue;
					});

				if (bStopQueue && TaskQueue.empty()) 
				{
					return;
				}

				Task = move(TaskQueue.front());
				TaskQueue.pop();
			}

			Task();
		}
	}

}