#pragma once

#include "core/SThreadPool.hpp"

#include <unordered_map>
#include <memory>

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
		Ref<T> GetAsset(const SAssetPath& Path) const
		{
			if (!IsValidPath(Path))
			{
				return {};
			}

			if (AssetCache.contains(Path))
			{
				Ref<T> AssetRef = AssetCache.at(Path).GetAs<T>();
				return AssetRef;
			}

			T* NewAsset = new T{ Path };
			Ref<T> NewAssetRef{ NewAsset };
			AssetCache[Path] = { NewAsset };
			return NewAssetRef;
		}

		template<typename T>
		T& GetAsset() const
		{
			return;
		}

		void LoadAsset(const Ref<SAsset>& Asset);

		void UnloadAsset(const Ref<SAsset>& Asset);

	public:

		SAssetPath GetAbsolutePath(const SAssetPath& RelativePath) const;

		SAssetPath GetRelativePath(const SAssetPath& AbsolutePath) const;

	private:
		
		SAssetPath AssetRootPath;

		SThreadPool IOThreadPool;

		mutable std::unordered_map<SAssetPath, Ref<SAsset>> AssetCache;

		int32 NumHandles = 0;
	};
}
