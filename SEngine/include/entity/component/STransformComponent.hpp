#pragma once

#include "SEntityComponent.hpp"

namespace Solus
{
	class SOLUS_API STransformComponent : public SEntityComponent
	{
	public:

		void SetTransform(const STransform& NewTransform)
		{
			Transform = NewTransform;
		}

		STransform GetTransform() const
		{
			return Transform;
		}

		STransform& GetTransform()
		{
			return Transform;
		}

	private:

		STransform Transform;

	};

	template<typename Archive>
	void serialize(Archive& archive, STransformComponent& TransformComp) {
		Vec3 Pos = TransformComp.GetTransform().GetPosition();
		Quaternion Quat = TransformComp.GetTransform().GetRotation();
		Vec3 Scale = TransformComp.GetTransform().GetScale();
		archive(Pos);
		archive(Quat);
		archive(Scale);
	}
}