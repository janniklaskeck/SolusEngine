#pragma once
#include "Camera.h"

namespace Solus
{

	class SOLUS_API FreeFlyCamera : public Camera
	{
	public:
		virtual Mat4f* GetViewMatrix() override;

		virtual bool Update() override;
	};

}

