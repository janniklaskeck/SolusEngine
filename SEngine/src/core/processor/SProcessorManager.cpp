#include "../../../include/core/processor/SProcessorManager.hpp"
#include "../../../include/core/processor/SProcessor.hpp"

namespace Solus
{

	void SProcessorManager::Tick(float DeltaTime)
	{
		for (SProcessor* Processor : Processors)
		{
			if (!Processor)
			{
				continue;
			}

			Processor->Tick(DeltaTime);
		}
	}

	void SProcessorManager::SortProcessors()
	{
		std::sort(Processors.begin(), Processors.end(), 
			[](SProcessor* First, SProcessor* Second) 
			{
				if (!First || !Second)
				{
					return false;
				}

				return First->GetPriority() < Second->GetPriority();
			});
	}

}
