#pragma once
#include "SolusObject.h"

#include "Matrix.h"
#include "Vector.h"
#include <cstdint>	

namespace Solus
{

	class SOLUS_API Entity : public SolusObject
	{
	public:
		Entity(Vec3f initialPosition = Vec3f(), Vec3f initalRotation = Vec3f());
		virtual ~Entity();

		void TEMP();

		virtual bool Update();

		virtual bool Render();

		uint64_t GetId() const
		{
			return entityId;
		}

		void SetPosition(Vec3f newPosition);
		void AddPosition(Vec3f newPosition);
		void SetRotation(Vec3f newRotation);
		void AddRotation(Vec3f deltaRotation);
		void SetScale(Vec3f newScale);
		void AddScale(Vec3f newScale);

		Vec3f GetPosition() const;
		Vec3f GetRotation() const;
		Vec3f GetScale() const;

		Vec3f GetForward() const;
		Vec3f GetRight() const;
		Vec3f GetUp() const;

		Mat4f GetTransform() const;

	protected:
		uint64_t entityId;

		class RenderMesh* mesh;
		Mat4f mTransform;

		Vec3f position;
		Vec3f rotation;
		Vec3f scale;
	};
}

