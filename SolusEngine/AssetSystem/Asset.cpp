#include "Asset.h"
#include "AssetMeta.h"
#include "Engine/Engine.h"
#include "Utility/FileUtils.h"
#include "AssetSystem/SAsset.h"

namespace Solus
{
	Asset::Asset()
	{
	}

	Asset::Asset(const Asset& other)
	{
		asset = other.asset;
		if (asset)
			asset->Increment();
	}

	Asset::Asset(Asset&& other) noexcept
	{
		asset = other.asset;
		other.asset = nullptr;
	}

	Asset::~Asset()
	{
		if (asset)
			asset->Decrement();
	}

	void Asset::Set(SAsset* _asset)
	{
		if (asset != _asset)
		{
			if (asset)
				asset->Decrement();
			asset = _asset;
			if (asset)
				asset->Increment();
		}
	}

	SAsset* Asset::Get() const
	{
		if (asset)
			return asset;
		return nullptr;
	}

	Asset& Asset::operator=(const Asset& other)
	{
		if (this != &other)
		{
			if (asset)
				asset->Decrement();

			asset = other.asset;
			if (asset)
				asset->Increment();
		}
		return *this;
	}

	Asset& Asset::operator=(Asset&& other) noexcept
	{
		if (this != &other)
		{
			if (asset)
				asset->Decrement();

			asset = other.asset;
			other.asset = nullptr;
		}
		return *this;
	}

	Asset& Asset::operator=(SAsset* ptr)
	{
		if (asset)
			asset->Decrement();
		asset = ptr;
		if (asset)
			asset->Increment();
		return *this;
	}

	SUUID Asset::GetId() const
	{
		if (asset)
			return asset->GetAssetId();
		return SUUID::DEFAULT;
	}

	bool Asset::IsValid() const
	{
		return asset;
	}

	SAsset* Asset::operator->() const
	{
		return asset;
	}

	SAsset& Asset::operator*() const
	{
		return *asset;
	}

	bool Asset::operator==(const Asset& other) const
	{
		return asset == other.asset;
	}

	bool Asset::operator!=(const Asset& other) const
	{
		return !(*this == other);
	}

	Asset::operator bool() const
	{
		return IsValid();
	}

}
