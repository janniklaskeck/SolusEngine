#pragma once
#include <bgfx/bgfx.h>

namespace Solus
{

	class SUniformManager
	{
	public:

		void Destroy();

		void CreateUniform(const SString& UniformName, const bgfx::UniformType::Enum Type);

		void SetUniform(const SString& UniformName, void* ValuePtr);

		bgfx::UniformHandle GetUniformHandle(const std::string& UniformName) const;

	private:

		struct SUniformData
		{
			bgfx::UniformType::Enum Type;
			bgfx::UniformHandle Handle;
		};

		std::unordered_map<std::string, SUniformData> UniformDataMap;
	};

}