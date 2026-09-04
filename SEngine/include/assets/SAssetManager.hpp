#pragma once

#include "core/SThreadPool.hpp"

namespace Solus
{
	class SAssetManager final
	{
		friend class SAsset;

	public:
		SAssetManager() = default;

		static SAssetManager& Get();

		void Init(const SAssetPath& AssetRootPath, const uint8 IOThreadPoolSize);

	public:

		bool IsValidPath(const SAssetPath& Path) const;

		bool ReadFile(const SAssetPath& Path, std::vector<uint8>& OutData);

		bool WriteFile(const SAssetPath& Path, const std::vector<uint8>& Data);

		template<typename T>
		Ref<T> GetAsset(const SAssetHandle Handle = SOLUS_INVALID_HANDLE) const
		{
			if (Handle != SOLUS_INVALID_HANDLE && AssetCache.contains(Handle))
			{
				Ref<T> AssetRef = AssetCache.at(Handle).GetAs<T>();
				return AssetRef;
			}

			T* NewAsset = new T;
			SAssetHandle NewHandle = AssetCache.size();
			NewAsset->Handle = NewHandle;

			Ref<T> NewAssetRef{ NewAsset };
			AssetCache[NewHandle] = { NewAsset };
			return NewAssetRef;
		}

		void LoadAsset(const Ref<SAsset>& Asset);

		void UnloadAsset(const Ref<SAsset>& Asset);

	public:

		SAssetPath GetAbsolutePath(const SAssetPath& RelativePath) const;

		SAssetPath GetRelativePath(const SAssetPath& AbsolutePath) const;

	private:
		
		SAssetPath AssetRootPath;

		SThreadPool IOThreadPool;

		mutable std::unordered_map<SAssetHandle, Ref<SAsset>> AssetCache;

		int32 NumHandles = 0;
	};
}
