#pragma once
#include "Object/Entity.h"

#include "Camera.generated.h"

namespace Solus
{
	SOLUS_CLASS();
	class SOLUS_API Camera : public Entity
	{
		REFLECT(Camera)
	public:
		Camera(float fovY = 90.f, float windowWidth = 1280.f, float windowHeight = 720.f, float near = .1f, float far = 1000.f);

		virtual Mat4f* GetViewMatrix() = 0;

		void UpdateProjectionMatrix(float fovY, float windowWidth, float windowHeight, float near = .1f, float far = 1000.f);

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
	protected:
		Mat4f projectionMatrix;
		SPROPERTY();
		bool isInputEnabled = false;
	};

}