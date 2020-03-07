#pragma once

#include "SComponent.generated.h"
#include "Engine/SolusEngine.h"

#include "Utility/RTTI.h"

namespace Solus
{
	class Entity;

	SOLUS_CLASS();
	class SOLUS_API SComponent
	{
		REFLECT(SComponent)
	public:
		void Attach(Entity* parent);
		virtual void BeginPlay() = 0;
		virtual void Update(float deltaTime) = 0;
		virtual void EndPlay() = 0;

	protected:
		Entity* parent = nullptr;
	};
}