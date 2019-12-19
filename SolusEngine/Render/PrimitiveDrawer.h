#pragma once

#include "Engine/SolusEngine.h"

#include "Utility/Vector.h"

namespace Solus
{
	class SOLUS_API PrimitiveDrawer
	{
	public:
		virtual void RenderRectangle(Vec2i center, Vec2i extents, Vec4f color) = 0;
		virtual void RenderCircle(Vec2i center, float radius, Vec4f color, int sections = 16) = 0;

		virtual void RenderSphere(Vec3f center, float radius, Vec4f color, int sections = 16) = 0;
		virtual void RenderBox(Vec3f center, Vec3f extents, Vec4f color) = 0;

		void SetWireframe(bool useWireframe);

	protected:
		bool useWireframe;
	};
}
