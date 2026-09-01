#pragma once
#include <functional>
#include <mutex>
#include <thread>
#include <queue>

namespace Solus
{
	class SThreadPool final
	{
	public:
		SThreadPool() = default;
		~SThreadPool();

		SThreadPool(const SThreadPool&) = delete;
		SThreadPool(SThreadPool&&) = delete;

		void operator=(const SThreadPool&) = delete;
		void operator=(SThreadPool&&) = delete;

		void InitThreads(const uint8 ThreadAmount = std::thread::hardware_concurrency());

		void EnqueueTask(std::function<void()> Task);

	private:

		void ThreadLoop();

	private:
		std::vector<std::thread> Threads;

		std::queue<std::function<void()> > TaskQueue;

		std::mutex TaskQueueMutex;

		std::condition_variable TaskQueueStateCondition;
		bool bStopQueue = false;
	};
}