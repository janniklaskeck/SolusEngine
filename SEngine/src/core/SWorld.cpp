#include "core/SWorld.hpp"

#include "entity/component/STransformComponent.hpp"

#include <cereal/archives/json.hpp>
#include "entity/component/SRigidBodyComponent.hpp"

#include <sstream>

namespace Solus
{
	SWorld::SWorld()
	{
	}

	SWorld::~SWorld()
	{

	}

	void SWorld::Tick(float DeltaTime)
	{
		
	}

	SEntity SWorld::CreateEntity(glm::vec3 Position, const std::string& Tag)
	{
		SEntity Entity = SEntity({ Registry, Registry.create() }, this);

		STransformComponent& TransformComp = Entity.AddComponent<STransformComponent>();
		TransformComp.SetTransform({ Position });

		STagComponent& TagComp = Entity.AddComponent<STagComponent>();
		TagComp.Tag = Tag;

		return Entity;
	}

	void SWorld::Serialize()
	{
		std::stringstream StringStream;
		{
			cereal::JSONOutputArchive Output{ StringStream };

			entt::snapshot{ Registry }
				.get<entt::entity>(Output)
				.get<STransformComponent>(Output);
				//.get<SRigidBodyComponent>(Output);
		}

		Registry.clear();

		cereal::JSONInputArchive Input{ StringStream };

		entt::snapshot_loader{ Registry }
			.get<entt::entity>(Input)
			.get<STransformComponent>(Input);
	}

	void SWorld::Deserialize()
	{

	}

}