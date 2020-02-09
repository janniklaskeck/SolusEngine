#pragma once
#include "Camera.h"
#include "FreeflyCamera.generated.h"


namespace Solus
{
	SOLUS_CLASS();
	class SOLUS_API FreeFlyCamera : public Camera
	{
		REFLECT(FreeFlyCamera) 
	public:
		virtual Mat4f* GetViewMatrix() override;

		virtual void Update(float deltaTime) override;

		SPROPERTY();
		float movementSpeed = 5.f;

	};

}

