#pragma once

#include <bgfx/bgfx.h>

namespace Solus
{

	class SMeshAsset : public SAsset
	{
	public:

		SMeshAsset() = default;

		SMeshAsset(const SAssetPath& InPath)
			: SAsset(InPath)
		{
		}

		void Load() override;

		void Unload() override;

		bool IsValid() const override;

		bool IsLoaded() const override;

	public:

		bgfx::VertexBufferHandle GetVertexBufferHandle() const
		{
			return VertexBufferHandle;
		}

		bgfx::IndexBufferHandle GetIndexBufferHandle() const
		{
			return IndexBufferHandle;
		}

	private:

		std::vector<SVertex> VertexData;
		std::vector<uint16> IndexData;
		SAABB Bounds{};

		bgfx::VertexBufferHandle VertexBufferHandle{ BGFX_INVALID_HANDLE };
		bgfx::IndexBufferHandle IndexBufferHandle{ BGFX_INVALID_HANDLE };

	};

}