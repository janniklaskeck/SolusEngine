#pragma once

#include "Engine/SolusEngine.h"
#include "Vector.h"
#include "Matrix.h"

namespace Solus
{
	class SOLUS_API Transform
	{
	public:
		Transform()
		{
			mTransform = Mat4f();
		}

		void Scale(float s)
		{
			Scale(s, s, s);
		}

		void Scale(const Vec3f& scale)
		{
			Scale(scale.x, scale.y, scale.z);
		}

		void Scale(float ScaleX, float ScaleY, float ScaleZ)
		{
		}

		void WorldPos(float x, float y, float z)
		{
		}

		void WorldPos(const Vec3f& Pos)
		{
		}

		void Rotate(float RotateX, float RotateY, float RotateZ)
		{
		}

		void Rotate(const Vec3f& r)
		{
			Rotate(r.x, r.y, r.z);
		}

		const Mat4f* GetTrans()
		{
			return &mTransform;
		}

	private:
	public: // TEMP
		Mat4f mTransform;
	};
}
