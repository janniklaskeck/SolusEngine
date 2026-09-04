#pragma once

#include <entt/entt.hpp>
#include "../core/SWorld.hpp"

namespace Solus
{

	class SWorld;
	
	class SOLUS_API SEntity
	{
	public:

		SEntity() = default;
		SEntity(const entt::handle InEntityHandle, SWorld* InWorld);

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			T& Component = EntityHandle.emplace<T>(std::forward<Args>(args)...);
			Component.Owner = *this;
			return Component;
		}

		template<typename T>
		T& GetComponent() const
		{
			return EntityHandle.get<T>();
		}

		template<typename T>
		bool HasComponent() const
		{
			return EntityHandle.all_of<T>();
		}

		template<typename T>
		void RemoveComponent()
		{
			EntityHandle.remove<T>();
		}

		void Destroy();

		bool IsValid() const
		{
			return EntityHandle.valid();
		}

	private:
		entt::handle EntityHandle = {};
		

		SWorld* World = nullptr;
	};
}