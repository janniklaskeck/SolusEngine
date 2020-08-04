#include "World.h"

#include "Entity.h"
#include "Component/MeshComponent.h"
#include "Engine/Engine.h"

#include "Utility/SerializeUtil.h"

namespace Solus
{

	SCLASS_IMPL(World);
	World::World()
	{
	}

	World::~World()
	{
		for (auto it = globalEntities.begin(); it != globalEntities.end(); it++)
		{
			delete it->second;
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
		for (auto it = globalEntities.begin(); it != globalEntities.end(); it++)
		{
			auto entry = *it;
			entities.push_back(entry.second);
		}
		return entities;
	}

	void World::BeginPlay()
	{
		for (auto it = globalEntities.begin(); it != globalEntities.end(); it++)
		{
			it->second->BeginPlay();
		}
	}

	void World::Update(float deltaTime)
	{
		for (auto it = globalEntities.begin(); it != globalEntities.end(); it++)
		{
			it->second->Update(deltaTime);
		}
	}

	void World::EndPlay()
	{
		for (auto it = globalEntities.begin(); it != globalEntities.end(); it++)
		{
			it->second->EndPlay();
		}
	}

	void World::Render()
	{
		for (auto it = globalEntities.begin(); it != globalEntities.end(); it++)
		{
			auto* meshComponent = it->second->GetComponent<MeshComponent>();
			if (meshComponent)
			{
				meshComponent->Render();
			}
		}
	}

	void World::SaveToFile() const
	{
		if (worldAsset)
		{

		}
		else
		{
			ArchiveStream stream("c:\\Spiele\\test.txt", ArchiveMode::WRITE);
			Serialize(stream);
		}
	}

	void World::ReadFromFile(const std::string& path)
	{
		ArchiveStream stream(path);
		Deserialize(stream);
	}
}
