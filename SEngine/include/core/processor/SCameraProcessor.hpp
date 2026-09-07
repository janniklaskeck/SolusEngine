#pragma once
#include "SProcessor.hpp"

namespace Solus
{
	class SCameraComponent;
	class STransformComponent;

	class SCameraProcessor : public SProcessor
	{
	public:

		SCameraProcessor();

		virtual void Tick(float DeltaTime) override;

		virtual int32 GetPriority() const override;

	private:

		virtual void TickCamera(float DeltaTime, STransform& Transform, SCameraComponent& Camera);

		virtual void TickInput(float DeltaTime, STransform& Transform, SCameraComponent& Camera);

	};
}