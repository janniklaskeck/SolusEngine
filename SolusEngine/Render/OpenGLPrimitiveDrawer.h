#pragma once
#include "PrimitiveDrawer.h"

namespace Solus
{

	class OpenGLShader;

	class OpenGLPrimitiveDrawer : public PrimitiveDrawer
	{
	public:
		OpenGLPrimitiveDrawer();

		virtual void RenderRectangle(Vec2i center, Vec2i extents, Vec4f color) override;
		virtual void RenderCircle(Vec2i center, float radius, Vec4f color, int sections = 16) override;

		virtual void RenderSphere(Vec3f center, float radius, Vec4f color, int sections = 16) override;
		virtual void RenderBox(Vec3f center, Vec3f extents, Vec4f color) override;

	private:
		unsigned int rectangleVAO = 0;
		unsigned int rectangleVBO = 0;
		OpenGLShader* rectangleShader;
	};

}
