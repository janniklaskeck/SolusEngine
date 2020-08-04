#pragma once
#include "SolusObject.h"
#include "Entity.generated.h"

#include "Component/SComponent.h"
#include "Utility/Matrix.h"
#include "Utility/Vector.h"

#include <cstdint>

namespace Solus
{
	SOLUS_CLASS();
	class SOLUS_API Entity : public SolusObject
	{
		META(Entity, SolusObject)
	public:

		Entity();
		Entity(Vec3f initialPosition, Vec3f initalRotation);
		virtual ~Entity();
		
		void AttachComponent(SComponent* component);
		virtual void BeginPlay();
		virtual void Update(float deltaTime);
		virtual void EndPlay();

		uint64_t GetId() const
		{
			return instanceId;
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

		template<typename T>
		T* GetComponent() const
		{
			for (const SComponent* component : components)
			{
				if (component->get_type() == rttr::type::get<T>())
					return (T*)component;
			}
			return nullptr;
		}

		const std::vector<SComponent*>& GetComponents() const;
		SPROPERTY();
		std::vector<SComponent*> components;
	protected:

		bool hasBegunPlay = false;


		uint64_t instanceId;

		Mat4f mTransform;

		SPROPERTY();
		Vec3f position = Vec3f(0.0f);
		SPROPERTY();
		Vec3f rotation = Vec3f(0.0f);
		SPROPERTY();
		Vec3f scale = Vec3f(1.0f);
	};
}

