#pragma once
#include "Object/SolusObject.h"
#include "Object/Entity.h"

#include "Utility/Vector.h"
#include <unordered_map>
#include <vector>

namespace Solus
{

	class SOLUS_API World : public SolusObject
	{
	public:
		World();
		~World();

		template<class T>
		T* SpawnEntity(Vec3f position = Vec3f(.0f), Vec3f rotation = Vec3f(.0f), Vec3f scale = Vec3f(1.f));

		bool DestroyEntity(Entity* entity);
		std::vector<Entity*> GetEntities() const;

		virtual void BeginPlay() override;
		virtual void Update(float deltaTime) override;
		virtual void EndPlay() override;
		void Render();

	protected:
		std::unordered_map<uint64_t, Entity*>* globalEntities;
	};

	template<class T>
	inline T* World::SpawnEntity(Vec3f position, Vec3f rotation, Vec3f scale)
	{
		T* instance = new T;
		Entity* entity = dynamic_cast<Entity*>(instance);
		if (!entity)
		{
			delete instance;
			return nullptr;
		}
		entity->SetPosition(position);
		entity->SetRotation(rotation);
		entity->SetScale(scale);
		//entity->BeginPlay();
		globalEntities->insert({ {entity->GetId(), entity } });
		return instance;
	}

}

