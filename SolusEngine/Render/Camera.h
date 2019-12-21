#pragma once
#include "Engine/SolusEngine.h"
#include "Object/Entity.h"

namespace Solus
{

	class SOLUS_API Camera : public Entity
	{
	public:

		Camera(float fovY = 90.f, float windowWidth = 1280.f, float windowHeight = 720.f, float near = .1f, float far = 1000.f);

		virtual Mat4f* GetViewMatrix() = 0;

		Mat4f* GetProjectionMatrix()
		{
			return &projectionMatrix;
		}

		void SetInputEnabled(bool isEnabled)
		{
			isInputEnabled = isEnabled;
		}

		bool IsInputEnabled() const
		{
			return isInputEnabled;
		}

	private:
		Mat4f projectionMatrix;
		bool isInputEnabled = true;
	};

}