#pragma once

#include <entt/entt.hpp>

namespace Solus
{

	class SOLUS_API SWorld final
	{
		friend class SEntity;

	public:

		SWorld();
		~SWorld();

		void Tick(float DeltaTime);

		SEntity CreateEntity(Vec3 Position = {});

		entt::registry& GetRegistry()
		{
			return Registry;
		}

		void Serialize();

		void Deserialize();


	private:

		entt::registry Registry;
	};
}