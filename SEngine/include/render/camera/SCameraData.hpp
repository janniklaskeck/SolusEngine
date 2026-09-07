#pragma once

namespace Solus
{

	enum class SCameraType : uint8
	{
		Perspective = 0,
		Orthographic
	};

	struct SCameraData
	{
		SCameraType Type = SCameraType::Perspective;

		Vec4 OrthographicArea{};

		float FOV_Y = 0.f;

		float NearPlane = 0.f;
		float FarPlane = 0.f;
	};

}