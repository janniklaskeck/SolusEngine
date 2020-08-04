#pragma once

#include "SComponent.generated.h"
#include "Object/SolusObject.h"
#include "Engine/SolusEngine.h"

#include "Utility/RTTI.h"

namespace Solus
{
	class Entity;

	SOLUS_CLASS();
	class SOLUS_API SComponent : public SolusObject
	{
		META(SComponent, SolusObject)
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