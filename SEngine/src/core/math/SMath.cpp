#include "core/math/SMath.hpp"

namespace Solus
{

	void STransform::SetRotation(const Vec3 Rotation)
	{
		Quaternion QuatYaw = glm::angleAxis(Math::DegToRad(Rotation.y), Vec::UpVector);
		Quaternion QuatPitch = glm::angleAxis(Math::DegToRad(Rotation.x), Vec::RightVector);

		SetRotation(QuatYaw * QuatPitch);
	}

	void STransform::SetRotation(const Quaternion NewRotation)
	{
		Rotation = glm::normalize(NewRotation);
	}

	Vec3 STransform::GetForwardVector() const
	{
		const Vec3 ForwardVector = glm::normalize(Rotation * Vec::ForwardVector);
		return ForwardVector;
	}

	Vec3 STransform::GetUpVector() const
	{
		const Vec3 UpVector = glm::normalize(Rotation * Vec::UpVector);
		return UpVector;
	}

	Vec3 STransform::GetRightVector() const
	{
		const Vec3 RightVector = glm::normalize(Rotation * Vec::RightVector);
		return RightVector;
	}

	glm::mat4 STransform::GetMatrix() const
	{
		glm::mat4 model =
			glm::translate(glm::mat4(1.0f), Position)
			* glm::mat4_cast(Rotation)
			* glm::scale(glm::mat4(1.0f), Scale);

		return model;
	}

}