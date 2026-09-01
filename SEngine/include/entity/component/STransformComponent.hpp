#pragma once

#include "SEntityComponent.hpp"

namespace Solus
{
	class SOLUS_API STransformComponent : public SEntityComponent
	{
	public:

		void SetTransform(const STransform& NewTransform)
		{
			Transform = NewTransform;
		}

		STransform GetTransform() const
		{
			return Transform;
		}

		STransform& GetTransform()
		{
			return Transform;
		}

	private:

		STransform Transform;

	};
}