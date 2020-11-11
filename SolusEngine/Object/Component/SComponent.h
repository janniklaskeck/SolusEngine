#pragma once

#include "SComponent.generated.h"
#include "Object/SObject.h"
#include "Engine/SolusEngine.h"

#include "Utility/RTTI.h"

namespace Solus
{
	class Entity;

	SOLUS_CLASS();
	class SOLUS_API SComponent : public SObject
	{
		META(SComponent, SObject)
	public:

		virtual void BeginPlay();
		virtual void Update(float deltaTime);
		virtual void EndPlay();

		void Attach(Entity* parent);

	protected:
		bool hasBegunPlay = false;

		Entity* parent = nullptr;
	};
}