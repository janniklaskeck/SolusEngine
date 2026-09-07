#include "../../include/render/SUniformManager.hpp"

namespace Solus
{


	void SUniformManager::Destroy()
	{
		for (auto& DataPair : UniformDataMap)
		{
			bgfx::UniformHandle Handle = DataPair.second.Handle;
			bgfx::destroy(Handle);
		}
	}

	void SUniformManager::CreateUniform(const SString& UniformName, const bgfx::UniformType::Enum Type)
	{
		if (!UniformDataMap.contains(UniformName))
		{
			bgfx::UniformHandle Handle = bgfx::createUniform(UniformName.c_str(), Type);

			UniformDataMap[UniformName] = {Type, Handle};
			return;
		}

		SLOG("Uniform %s already exists!", UniformName.c_str());
	}

	void SUniformManager::SetUniform(const SString& UniformName, const void* ValuePtr)
	{
		if (!UniformDataMap.contains(UniformName))
		{
			SLOG("Uniform %s does not exist!", UniformName.c_str());
			return;
		}
		const SUniformData& Data = UniformDataMap[UniformName];

		bgfx::setUniform(Data.Handle, ValuePtr);
	}

	bgfx::UniformHandle SUniformManager::GetUniformHandle(const std::string& UniformName) const
	{
		if (!UniformDataMap.contains(UniformName))
		{
			return BGFX_INVALID_HANDLE;
		}

		return UniformDataMap.at(UniformName).Handle;
	}

}
