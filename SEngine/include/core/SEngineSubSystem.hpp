#pragma once

namespace Solus
{

	class SEngine;

	class SEngineSubSystem
	{
	public:

		SEngineSubSystem() = default;

		virtual ~SEngineSubSystem() = default;

		SEngineSubSystem(const SEngineSubSystem&) = delete;
		SEngineSubSystem(const SEngineSubSystem&&) = delete;

		void operator=(const SEngineSubSystem&) = delete;
		void operator=(const SEngineSubSystem&&) = delete;

		virtual bool Initialize(SEngine* InEngine)
		{
			Engine = InEngine;
			return true;
		}

		virtual void Tick(const float DeltaTime)
		{ }

		virtual void Shutdown()
		{ }

	protected:

		SEngine* Engine = nullptr;

	};

}
