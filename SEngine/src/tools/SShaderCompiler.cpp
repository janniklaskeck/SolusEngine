
#include "tools/SShaderCompiler.hpp"

#include "assets/SAssetManager.hpp"

#include <bx/bx.h>

#include <cstdlib>
#include <format>
#include <windows.h>


namespace Solus
{
	bool SShaderCompiler::CompileShader(const SAssetPath& FilePath, const SShaderType Type, SAssetPath& OutAssetPath)
	{
		if (!SAssetManager::Get().IsValidPath(FilePath))
		{
			return false;
		}

		const SString ShaderCompilerPath = (std::filesystem::current_path() / "shaderc.exe").string();

		const SString AbsolutePath = SAssetManager::Get().GetAbsolutePath(FilePath).string();

		const SString TypeString = [Type]()
			{
				switch (Type)
				{
				case SShaderType::Vertex:
					return "Vertex";
				case SShaderType::Fragment:
					return "Fragment";
				case SShaderType::Compute:
					return "Compute";
				}

				return "ERROR";
			}();

		const SString ShaderIncludePath = SAssetManager::Get().GetAbsolutePath("shaders").string();
		const SString VaringDefPath = SAssetManager::Get().GetAbsolutePath("shaders/varying.def.sc").string();
		const SString PlatformParam = "--platform windows";
		const SString ProfileString = "-p spirv";

		const SString OutputPath = SAssetManager::Get().GetAbsolutePath(FilePath).replace_extension(FilePath.extension().string() + ".bin").string();

		SString Command = std::format("{} -i {} --varyingdef {} --type {} {} {} -f {} -o {}", 
			ShaderCompilerPath,
			ShaderIncludePath,
			VaringDefPath,
			TypeString,
			PlatformParam,
			ProfileString,
			AbsolutePath,
			OutputPath);

		STARTUPINFOA si{};
		si.cb = sizeof(si);
		PROCESS_INFORMATION pi{};

		BOOL bWasSuccessful = CreateProcessA(
			nullptr, 
			Command.data(),
			nullptr, 
			nullptr, 
			FALSE,
			CREATE_NO_WINDOW,
			nullptr, 
			nullptr,
			&si, 
			&pi
		);

		if (bWasSuccessful) 
		{
			WaitForSingleObject(pi.hProcess, INFINITE);
			CloseHandle(pi.hProcess);
			CloseHandle(pi.hThread);
		}

		if (!bWasSuccessful)
		{
			return false;
		}

		OutAssetPath = OutputPath;

		return SAssetManager::Get().IsValidPath(OutAssetPath);
	}
}
