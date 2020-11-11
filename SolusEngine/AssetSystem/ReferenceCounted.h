#pragma once

#include "Engine/SolusEngine.h"

#include <atomic>

namespace Solus
{
	class SOLUS_API ReferenceCounted
	{
	public:
		void Increment();

		void Decrement();

		int GetRefCount() const;

	private:
		std::atomic_int refCount = 0;
	};

}
