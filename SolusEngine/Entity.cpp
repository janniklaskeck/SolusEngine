#include "Entity.h"

#include "Engine.h"
#include "Helper.h"

#include "RenderDevice.h"
#include "RenderMesh.h"
#include "FreeFlyCamera.h"

#include <glm/gtx/matrix_decompose.hpp>

namespace Solus
{

	Entity::Entity(Vec3f initialPosition, Vec3f initialRotation)
		: mesh(nullptr)
	{
		mTransform = Mat4f(1.f);
		mTransform = glm::rotate(mTransform, initialRotation.x, Vec3f(1, 0, 0));
		mTransform = glm::rotate(mTransform, initialRotation.y, Vec3f(0, 1, 0));
		mTransform = glm::rotate(mTransform, initialRotation.z, Vec3f(0, 0, 1));

		mTransform = glm::translate(mTransform, initialPosition);
		mTransform = glm::scale(mTransform, Vec3f(1, 1, 1));

		entityId = GenerateUUID();
	}

	Entity::~Entity()
	{}

	void Entity::TEMP()
	{
		mesh = Engine::Instance()->GetRenderDevice()->CreateMesh("");
		mesh->owner = this;
	}

	bool Entity::Update()
	{
		return false;
	}

	bool Entity::Render()
	{
		if (mesh)
			mesh->Render();
		return false;
	}

	void Entity::SetPosition(Vec3f newPosition)
	{
		mTransform[3].x = newPosition.x;
		mTransform[3].y = newPosition.y;
		mTransform[3].z = newPosition.z;
	}

	void Entity::SetRotation(Vec3f newRotation)
	{

	}

	void Entity::AddRotation(Vec3f deltaRotation)
	{
		mTransform = glm::rotate(mTransform, deltaRotation.x, Vec3f(1, 0, 0));
		mTransform = glm::rotate(mTransform, deltaRotation.y, Vec3f(0, 1, 0));
		mTransform = glm::rotate(mTransform, deltaRotation.z, Vec3f(0, 0, 1));
	}

	void Entity::SetScale(Vec3f newScale)
	{
		mTransform = glm::scale(mTransform, newScale);
	}

	Vec3f Entity::GetPosition() const
	{
		glm::vec3 scale;
		glm::quat rotation;
		glm::vec3 translation;
		glm::vec3 skew;
		glm::vec4 perspective;
		glm::decompose(mTransform, scale, rotation, translation, skew, perspective);
		return translation;
	}

	Vec3f Entity::GetRotation() const
	{
		glm::vec3 scale;
		glm::quat rotation;
		glm::vec3 translation;
		glm::vec3 skew;
		glm::vec4 perspective;
		glm::decompose(mTransform, scale, rotation, translation, skew, perspective);
		Vec3f rotationEuler = glm::eulerAngles(rotation);
		return rotationEuler;
	}

	Vec3f Entity::GetScale() const
	{
		glm::vec3 scale;
		glm::quat rotation;
		glm::vec3 translation;
		glm::vec3 skew;
		glm::vec4 perspective;
		glm::decompose(mTransform, scale, rotation, translation, skew, perspective);
		return scale;
	}

	Vec3f Entity::GetForward() const
	{

		Vec3f rotation = GetRotation();
		float cosPitch = glm::cos(rotation.x);
		float sinPitch = glm::sin(rotation.x);
		float cosYaw = glm::cos(rotation.z);
		float sinYaw = glm::sin(rotation.z);

		glm::vec3 zaxis = { sinYaw * cosPitch, -sinPitch, cosPitch * cosYaw };
		return zaxis;
	}

	Vec3f Entity::GetRight() const
	{
		Vec3f rotation = GetRotation();
		float cosYaw = glm::cos(rotation.z);
		float sinYaw = glm::sin(rotation.z);

		glm::vec3 xaxis = { cosYaw, 0, -sinYaw };
		return -xaxis;
	}

	Vec3f Entity::GetUp() const
	{
		Vec3f rotation = GetRotation();
		float cosPitch = glm::cos(rotation.x);
		float sinPitch = glm::sin(rotation.x);
		float cosYaw = glm::cos(rotation.z);
		float sinYaw = glm::sin(rotation.z);

		glm::vec3 yaxis = { sinYaw * sinPitch, cosPitch, cosYaw * sinPitch };
		return -yaxis;
	}
}