#include "World.h"

#include "Entity.h"

namespace Solus
{

	World::World()
	{
		globalEntities = new std::unordered_map<uint64_t, Entity*>;
	}

	World::~World()
	{
		for (auto it = globalEntities->begin(); it != globalEntities->end(); it++)
		{
			auto entry = *it;
			delete entry.second;
		}
	}

	bool World::DestroyEntity(Entity* entity)
	{
		if (entity)
		{
			globalEntities->erase(entity->GetId());
			delete entity;
			return true;
		}
		return false;
	}

	std::vector<class Entity*> World::GetEntities() const
	{
		std::vector<Entity*> entities;
		for (auto it = globalEntities->begin(); it != globalEntities->end(); it++)
		{
			auto entry = *it;
			entities.push_back(entry.second);
		}
		return entities;
	}

	void World::Update()
	{
		for (auto it = globalEntities->begin(); it != globalEntities->end(); it++)
		{
			it->second->Update();
		}
	}

	void World::Render()
	{
		for (auto it = globalEntities->begin(); it != globalEntities->end(); it++)
		{
			it->second->Render();
		}
	}

}