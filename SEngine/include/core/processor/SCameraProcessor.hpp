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

		void RegisterCameraComponent(SCameraComponent& CameraComponent);

	private:

		virtual void TickCamera(float DeltaTime, STransformComponent& TransformComp, SCameraComponent& CameraComponent);

		virtual void TickInput(float DeltaTime, STransformComponent& TransformComp, SCameraComponent& CameraComponent);

		void SetupProjectionMatrix(SCameraComponent& CameraComponent);

		void SetViewProjectionMatrices(const STransformComponent& TransformComp, const SCameraComponent& CameraComponent);

	private:


	};
}