#include "assets/SAssetManager.hpp"
#include "SDL3/SDL_iostream.h"

namespace Solus
{
	SAssetManager& SAssetManager::Get()
	{
		static SAssetManager AssetManager;
		return AssetManager;
	}

	void SAssetManager::Init(const SAssetPath& AssetRootPath, const uint8 IOThreadPoolSize)
	{
		IOThreadPool.InitThreads(IOThreadPoolSize);
		SAssetManager::AssetRootPath = AssetRootPath;
	}

	bool SAssetManager::IsValidPath(const SAssetPath& Path) const
	{
		const SAssetPath FullPath = GetAbsolutePath(Path);
		return std::filesystem::exists(FullPath);
	}

	bool SAssetManager::ReadFile(const SAssetPath& Path, std::vector<uint8>& OutData)
	{
		const SAssetPath AbsolutePath = GetAbsolutePath(Path);
		if (!IsValidPath(AbsolutePath))
		{
			return false;
		}

		size_t AssetSize = 0;
		const SString PathString = AbsolutePath.string();
		void* AssetDataPtr = SDL_LoadFile(PathString.c_str(), &AssetSize);

		OutData.resize(AssetSize);
		SDL_memcpy(OutData.data(), AssetDataPtr, AssetSize);

		SDL_free(AssetDataPtr);
		return true;
	}

	bool SAssetManager::WriteFile(const SAssetPath& Path, const std::vector<uint8>& Data)
	{
		const SAssetPath FullPath = GetAbsolutePath(Path);

		const SString PathString = FullPath.string();
		const bool bSuccess = SDL_SaveFile(PathString.c_str(), Data.data(), Data.size());
		return bSuccess;
	}

	void SAssetManager::LoadAsset(const Ref<SAsset>& Asset)
	{
		if (!Asset || Asset->IsLoaded() || !Asset->IsValid())
		{
			return;
		}

		Asset->Load();
	}

	void SAssetManager::UnloadAsset(const Ref<SAsset>& Asset)
	{
		if (!Asset || !Asset->IsLoaded())
		{
			return;
		}

		Asset->Unload();
	}

	SAssetPath SAssetManager::GetAbsolutePath(const SAssetPath& RelativePath) const
	{
		if (RelativePath.is_absolute())
		{
			return RelativePath;
		}
		
		return AssetRootPath / RelativePath;
	}

	SAssetPath SAssetManager::GetRelativePath(const SAssetPath& AbsolutePath) const
	{
		if (AbsolutePath.is_relative())
		{
			return AbsolutePath;
		}

		return std::filesystem::relative(AbsolutePath, AssetRootPath);
	}

}