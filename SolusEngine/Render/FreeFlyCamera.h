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

		virtual bool Update() override;
		SPROPERTY();
		bool asd = true;

		void* Asd()
		{
			return &asd2;
		}
		//friend FreeFlyCamera_Reflection;
		//FreeFlyCamera_Reflection refl;
	};

}

