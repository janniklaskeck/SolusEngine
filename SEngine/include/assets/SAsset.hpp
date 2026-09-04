#pragma once


namespace Solus
{
	using SAssetHandle = uint32;
	const SAssetHandle InvalidAssetHandle = 0xffffffff;
#define SOLUS_INVALID_HANDLE InvalidAssetHandle

	class SAsset : public RefCounted
	{
		friend class SAssetManager;

	public:

		SAsset() = default;

		virtual void Load();

		virtual void Unload();

		virtual bool IsValid() const;

		virtual bool IsLoaded() const
		{
			return Size() > 0;
		}

	public:

		const uint8* Data() const
		{
			return Bytes.data();
		}

		uint32 Size() const
		{
			return Bytes.size();
		}

		SAssetHandle GetAssetHandle() const
		{
			return Handle;
		}

	protected:

		std::vector<uint8>& GetDataRef()
		{
			return Bytes;
		}

	protected:

		SAssetHandle Handle = SOLUS_INVALID_HANDLE;

		std::vector<uint8> Bytes;

	};

}