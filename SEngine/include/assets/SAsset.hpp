#pragma once


namespace Solus
{
	using SAssetHandle = uint32;
	const SAssetHandle InvalidAssetHandle = 0xffffffff;

	class SAsset : public RefCounted
	{
		friend class SAssetManager;

	public:

		SAsset() = default;

		SAsset(const SAssetPath& InPath)
			: Path(InPath)
		{}

		virtual void Load();

		virtual void Unload();

		virtual bool IsValid() const;

		virtual bool IsLoaded() const
		{
			return Size() > 0;
		}

	public:

		void SetPath(const SAssetPath& InPath)
		{
			Path = InPath;
		}

		SAssetPath GetPath() const
		{
			return Path;
		}

		const uint8* Data() const
		{
			return Bytes.data();
		}

		uint32 Size() const
		{
			return Bytes.size();
		}

	protected:

		std::vector<uint8>& GetDataRef()
		{
			return Bytes;
		}

	protected:

		SAssetPath Path;

		std::vector<uint8> Bytes;

		SAssetHandle Handle = InvalidAssetHandle;
	};

}