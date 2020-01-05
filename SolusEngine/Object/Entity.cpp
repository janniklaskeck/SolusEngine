#include "Entity.h"

#include "Engine/Engine.h"

#include "Render/RenderDevice.h"
#include "Render/RenderMesh.h"
#include "Render/FreeFlyCamera.h"

#include "Utility/Helper.h"

#include <glm/gtx/matrix_decompose.hpp>

namespace Solus
{
	REFLECT_STRUCT_BEGIN(Entity, SolusObject)
	REFLECT_STRUCT_MEMBER(entityId)
	REFLECT_STRUCT_MEMBER(position)
	REFLECT_STRUCT_MEMBER(rotation)
	REFLECT_STRUCT_MEMBER(scale)
	REFLECT_STRUCT_END()


	Entity::Entity(Vec3f initialPosition, Vec3f initialRotation)
		: mesh(nullptr)
	{
		mTransform = Mat4f(1.f);
		position = initialPosition;
		rotation = initialRotation;
		scale = Vec3f(1.0f);

		entityId = GenerateUUID();
	}

	Entity::~Entity()
	{}

	void Entity::TEMP()
	{
		auto* meshAsset = (MeshAsset*)gEngine->GetAssetManager()->GetAsset("Model/suzanne.obj");
		mesh = gEngine->GetRenderDevice()->CreateMesh(meshAsset);
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
		position = newPosition;
	}

	void Entity::AddPosition(Vec3f newPosition)
	{
		position += newPosition;
	}

	void Entity::SetRotation(Vec3f newRotation)
	{
		rotation = newRotation;
	}

	void Entity::AddRotation(Vec3f deltaRotation)
	{
		rotation += deltaRotation;
	}

	void Entity::SetScale(Vec3f newScale)
	{
		scale = newScale;
	}

	void Entity::AddScale(Vec3f newScale)
	{
		scale += newScale;
	}

	Vec3f Entity::GetPosition() const
	{
		return Vec3f(position);
	}

	Vec3f Entity::GetRotation() const
	{
		return Vec3f(rotation);
	}

	Vec3f Entity::GetScale() const
	{
		return Vec3f(scale);
	}

	Vec3f Entity::GetForward() const
	{
		return Vec3f(glm::cos(rotation.x) * glm::sin(rotation.y),
					 glm::sin(rotation.x),
					 glm::cos(rotation.x) * glm::cos(rotation.y));
	}

	Vec3f Entity::GetRight() const
	{
		return Vec3f(glm::sin(rotation.y - glm::half_pi<float>()),
					 0.0f,
					 glm::cos(rotation.y - glm::half_pi<float>()));
	}

	Vec3f Entity::GetUp() const
	{
		return Vec3f(glm::cross(GetRight(), GetForward()));
	}
	Mat4f Entity::GetTransform() const
	{
		return glm::lookAt(GetPosition(), GetPosition() + GetForward(), GetUp());
	}
}