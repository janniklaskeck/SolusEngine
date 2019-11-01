#pragma once
#include "SolusEngine.h"
#include "Entity.h"
#include "Engine.h"

namespace Solus
{

	class SOLUS_API Camera : public Entity
	{
	public:

		Camera(float fovY = 90.f, float windowWidth = 1280.f, float windowHeight = 720.f, float near = .1f, float far = 1000.f)
			: projectionMatrix(1.0f)
		{
			projectionMatrix = Mat4f(1.f);
			projectionMatrix = glm::perspective(glm::radians(fovY / 2.f), windowWidth / windowHeight, near, far);

			if (!Engine::Instance()->GetMainCamera())
				Engine::Instance()->SetMainCamera(this);
		}

		virtual Mat4f* GetViewMatrix() = 0;

		Mat4f* GetProjectionMatrix()
		{
			return &projectionMatrix;
		}

	private:
		Mat4f projectionMatrix;
	};

}