#include "ReferenceCounted.h"

namespace Solus
{
    void ReferenceCounted::Increment()
	{
		++refCount;
	}

	void ReferenceCounted::Decrement()
	{
		--refCount;
	}

	int ReferenceCounted::GetRefCount() const
	{
		return refCount;
	}
}
