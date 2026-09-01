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
		SEntity(const entt::entity InEntityHandle, SWorld* InWorld);

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			T& Component = World->Registry.emplace<T>(EntityHandle, std::forward<Args>(args)...);
			Component.Owner = *this;
			return Component;
		}

		template<typename T>
		T& GetComponent() const
		{
			return World->Registry.get<T>(EntityHandle);
		}

		template<typename T>
		bool HasComponent() const
		{
			return World->Registry.all_of<T>(EntityHandle);
		}

		template<typename T>
		void RemoveComponent()
		{
			World->Registry.remove<T>(EntityHandle);
		}

	private:
		entt::entity EntityHandle = entt::null;

		SWorld* World = nullptr;
	};
}