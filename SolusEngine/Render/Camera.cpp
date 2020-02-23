#include "Camera.h"

#include "Engine/Engine.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Solus
{
	SCLASS_IMPL(Camera);

	Camera::Camera(float fovY, float windowWidth, float windowHeight, float near, float far)
	{
		projectionMatrix = glm::perspective<float>(glm::radians(fovY / 2.f), windowWidth / windowHeight, near, far);

		if (!gEngine->GetMainCamera())
			gEngine->SetMainCamera(this);
	}

	void Camera::UpdateProjectionMatrix(float fovY, float windowWidth, float windowHeight, float near, float far)
	{
		projectionMatrix = glm::perspective<float>(glm::radians(fovY / 2.f), windowWidth / windowHeight, near, far);
	}

}

