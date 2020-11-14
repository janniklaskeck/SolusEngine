#include "World.h"

#include "Entity.h"
#include "Component/MeshComponent.h"
#include "Engine/Engine.h"

#include "Utility/SerializeUtil.h"

#include "AssetSystem/SAsset.h"

namespace Solus
{
	World::World()
	{}

	World::~World()
	{
		for (auto& entity : globalEntities)
		{
			delete entity.second;
		}
	}

	bool World::DestroyEntity(Entity* entity)
	{
		if (entity)
		{
			entity->EndPlay();
			globalEntities.erase(entity->GetId());
			delete entity;
			return true;
		}
		return false;
	}

	std::vector<class Entity*> World::GetEntities() const
	{
		std::vector<Entity*> entities;
		for (auto& entity : globalEntities)
		{
			entities.push_back(entity.second);
		}
		return entities;
	}

	void World::BeginPlay()
	{
		for (auto& entity : globalEntities)
		{
			entity.second->BeginPlay();
		}
	}

	void World::Update(float deltaTime)
	{
		for (auto& entity : globalEntities)
		{
			entity.second->Update(deltaTime);
		}
	}

	void World::EndPlay()
	{
		for (auto& entity : globalEntities)
		{
			entity.second->EndPlay();
		}
	}

	void World::Render()
	{
		for (auto& entity : globalEntities)
		{
			auto* meshComponent = entity.second->GetComponent<MeshComponent>();
			if (meshComponent)
			{
				meshComponent->Render();
			}
		}
	}

	bool World::SaveWorld(Asset _worldAsset)
	{
		worldAsset = _worldAsset;

		ArchiveStream stream(worldAsset->GetSourceFilePath().string(), ArchiveMode::WRITE);
		Serialize(stream);
		return true;
	}

	void World::LoadWorld(Asset _worldAsset)
	{
		worldAsset = _worldAsset;
		ArchiveStream stream(worldAsset->GetSourceFilePath().string());
		Deserialize(stream);
	}
}
