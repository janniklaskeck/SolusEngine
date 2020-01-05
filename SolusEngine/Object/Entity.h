#pragma once
#include "SolusObject.h"

#include "Entity.generated.h"

#include "Utility/Matrix.h"
#include "Utility/Vector.h"

#include <cstdint>

namespace Solus
{
	SOLUS_CLASS();
	class SOLUS_API Entity : public SolusObject
	{
		REFLECT(Entity)
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
		SPROPERTY();
		uint64_t entityId;

		class RenderMesh* mesh;
		Mat4f mTransform;

		SPROPERTY();
		Vec3f position;
		SPROPERTY();
		Vec3f rotation;
		SPROPERTY();
		Vec3f scale;
	};
}

