#include "Camera.h"

#include "Engine/Engine.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Solus
{

	Camera::Camera(float fovY, float windowWidth, float windowHeight, float nearPlane, float farPlane)
	{
		projectionMatrix = glm::perspective<float>(glm::radians(fovY / 2.f), windowWidth / windowHeight, nearPlane, farPlane);

		if (!gEngine->GetMainCamera())
			gEngine->SetMainCamera(this);
	}

	void Camera::UpdateProjectionMatrix(float fovY, float windowWidth, float windowHeight, float nearPlane, float farPlane)
	{
		projectionMatrix = glm::perspective<float>(glm::radians(fovY / 2.f), windowWidth / windowHeight, nearPlane, farPlane);
	}

}

