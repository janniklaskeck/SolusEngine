#pragma once

namespace Solus
{
	namespace Math
	{
		inline float DegToRad(float AngleInDegrees)
		{
			return glm::radians(AngleInDegrees);
		}

		inline float RadToDeg(float AngleInRadians)
		{
			return glm::degrees(AngleInRadians);
		}
	}

	struct STransform
	{
		STransform() = default;

		STransform(Vec3 InPosition)
			: Position(InPosition)
		{
		}

		void SetPosition(const Vec3 NewPosition)
		{
			Position = NewPosition;
		}

		void AddPosition(const Vec3 AddedPosition)
		{
			Position += AddedPosition;
		}

		Vec3 GetPosition() const
		{
			return Position;
		}

		void SetRotation(const Quaternion NewRotation);

		void SetRotation(const Vec3 Rotation);

		Quaternion GetRotation() const
		{
			return Rotation;
		}

		Vec3 GetRotationEuler() const
		{
			Vec3 EulerAngles = glm::degrees(glm::eulerAngles(Rotation));
			return EulerAngles;
		}

		Vec3 GetForwardVector() const;

		Vec3 GetUpVector() const;

		Vec3 GetRightVector() const;

		void SetScale(const Vec3 NewScale)
		{
			Scale = NewScale;
		}

		Vec3 GetScale() const
		{
			return Scale;
		}

		glm::mat4 GetMatrix() const;

	private:

		Vec3 Position{};
		Quaternion Rotation{ 1.f, 0.f, 0.f, 0.f };
		Vec3 Scale{ 1.f, 1.f, 1.f };
	};

	struct SAABB
	{
		Vec3 Min{};
		Vec3 Max{};

	};

}