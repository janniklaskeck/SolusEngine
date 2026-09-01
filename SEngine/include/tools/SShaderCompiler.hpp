#pragma once

namespace Solus
{

	enum class SShaderType : uint8
	{
		Vertex = 0,
		Fragment,
		Compute
	};

	class SShaderCompiler
	{
	public:

		static bool CompileShader(const SAssetPath& FilePath, const SShaderType Type, SAssetPath& OutAssetPath);
	};

}