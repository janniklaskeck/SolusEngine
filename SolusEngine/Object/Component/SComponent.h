#pragma once

#include "Engine/SolusEngine.h"

namespace Solus
{
	class Entity;

	class SOLUS_API SComponent
	{
	public:
		void Attach(Entity* parent);
		virtual void BeginPlay() = 0;
		virtual void Update(float deltaTime) = 0;
		virtual void EndPlay() = 0;

	private:
		Entity* parent = nullptr;
	};
}