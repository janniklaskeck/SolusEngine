#pragma once
#include "SProcessor.hpp"

namespace Solus
{

	class SPhysicsProcessor final : public SProcessor
	{
	public:

		void Tick(float DeltaTime) override;

		int32 GetPriority() const override;
	};

}

