#include "Camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Solus
{
	Camera::Camera(float fovY, float windowWidth, float windowHeight, float near, float far)
	{
		projectionMatrix = glm::perspective<float>(glm::radians(fovY / 2.f), windowWidth / windowHeight, near, far);

		if (!gEngine->GetMainCamera())
			gEngine->SetMainCamera(this);
	}

}

