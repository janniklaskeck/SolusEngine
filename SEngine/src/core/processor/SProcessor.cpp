#include "../../../include/core/processor/SProcessor.hpp"
#include "../../../include/core/processor/SProcessorManager.hpp"

namespace Solus
{

	int32 SProcessor::GetPriority() const
	{
		return ProcessorPriority::Default;
	}

}