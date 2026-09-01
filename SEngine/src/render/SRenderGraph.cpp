#include "render/SRenderGraph.hpp"



namespace Solus
{

	STextureHandle SPassBuilder::CreateTexture(const STextureDesc& desc)
	{
		STextureHandle h = Graph.AllocTextureHandle();
		STextureResource& res = Graph.TextureResources[h.ID];
		res.Description = desc;
		res.bIsImported = false;

		// Record as output of this pass
		Graph.RenderPassData[PassIndex].Writes.push_back(h);
		res.RefCount++;
		return h;

	}

	STextureHandle SPassBuilder::ReadTexture(STextureHandle handle)
	{
		assert(handle.IsValid());
		Graph.TextureResources[handle.ID].RefCount++;
		Graph.RenderPassData[PassIndex].Reads.push_back(handle);
		return handle;

	}

	STextureHandle SPassBuilder::WriteTexture(STextureHandle handle)
	{
		assert(handle.IsValid());
		Graph.RenderPassData[PassIndex].Writes.push_back(handle);
		return handle;

	}

	SBufferHandle SPassBuilder::CreateBuffer(const SBufferDesc& desc)
	{
		SBufferHandle h = Graph.AllocBufferHandle();
		SBufferResource& res = Graph.BufferResources[h.ID];
		res.Description = desc;
		res.bIsImported = false;
		Graph.RenderPassData[PassIndex].BufferWrites.push_back(h);
		res.RefCount++;
		return h;

	}

	SBufferHandle SPassBuilder::ReadBuffer(SBufferHandle handle)
	{
		assert(handle.IsValid());
		Graph.BufferResources[handle.ID].RefCount++;
		Graph.RenderPassData[PassIndex].BufferReads.push_back(handle);
		return handle;

	}

	SBufferHandle SPassBuilder::WriteBuffer(SBufferHandle handle)
	{
		assert(handle.IsValid());
		Graph.RenderPassData[PassIndex].BufferWrites.push_back(handle);
		return handle;

	}

	//////////////////////////////////////////////////////////////////////////

	STextureHandle SRenderGraph::ImportTexture(const SString& Name, bgfx::TextureHandle bgfxHandle, bgfx::FrameBufferHandle fbHandle /*= BGFX_INVALID_HANDLE*/)
	{
		STextureHandle h = AllocTextureHandle();
		STextureResource& res = TextureResources[h.ID];
		res.Description.Name = Name;
		res.Handle = bgfxHandle;
		res.FBHandle = fbHandle;
		res.bIsImported = true;
		res.RefCount = 1;  // imported resources are never culled
		return h;

	}

	SBufferHandle SRenderGraph::ImportBuffer(const SString& name, bgfx::DynamicVertexBufferHandle bgfxHandle)
	{
		SBufferHandle h = AllocBufferHandle();
		SBufferResource& res = BufferResources[h.ID];
		res.Description.Name = name;
		res.Handle = bgfxHandle;
		res.bIsImported = true;
		res.RefCount = 1;
		return h;

	}

	void SRenderGraph::Compile(uint16_t backbufferWidth, uint16_t backbufferHeight)
	{
		BBWidth = backbufferWidth;
		BBHeight = backbufferHeight;

		CullPasses();
		bCompiled = true;

	}

	void SRenderGraph::Execute()
	{
		assert(bCompiled && "Call compile() before execute()");
		CreatePhysicalResources();

		for (const SRenderPassData& PassData : RenderPassData) 
		{
			if (PassData.bCulled)
			{
				continue;
			}

			if (PassData.PassFunction) 
			{
				PassData.PassFunction(*this, PassData.ViewId);
			}
		}

	}

	void SRenderGraph::Reset()
	{
		DestroyResources();
		RenderPassData.clear();

		// Keep imported resources but reset their refcounts for re-tracking
		for (STextureResource& Resource : TextureResources)
		{
			if (Resource.bIsImported)
			{
				Resource.RefCount = 1;
			}
		}

		for (SBufferResource& Resource : BufferResources) 
		{
			if (Resource.bIsImported)
			{
				Resource.RefCount = 1;
			}
		}

		bCompiled = false;

	}

	bgfx::TextureHandle SRenderGraph::GetTexture(STextureHandle Handle) const
	{
		assert(Handle.IsValid() && Handle.ID < TextureResources.size());
		return TextureResources[Handle.ID].Handle;

	}

	bgfx::FrameBufferHandle SRenderGraph::GetFramebuffer(STextureHandle Handle) const
	{
		assert(Handle.IsValid() && Handle.ID < TextureResources.size());
		return TextureResources[Handle.ID].FBHandle;

	}

	bgfx::DynamicVertexBufferHandle SRenderGraph::GetBuffer(SBufferHandle Handle) const
	{
		assert(Handle.IsValid() && Handle.ID < BufferResources.size());
		return BufferResources[Handle.ID].Handle;

	}

	STextureHandle SRenderGraph::AllocTextureHandle()
	{
		STextureHandle Handle;
		Handle.ID = static_cast<uint16_t>(TextureResources.size());
		TextureResources.emplace_back();
		return Handle;

	}

	SBufferHandle SRenderGraph::AllocBufferHandle()
	{
		SBufferHandle Handle;
		Handle.ID = static_cast<uint16_t>(BufferResources.size());
		BufferResources.emplace_back();
		return Handle;

	}

	void SRenderGraph::CullPasses()
	{
		// Simple ref-count culling pass:
		// A pass is alive if at least one of its outputs has a non-zero refcount
		// (i.e. another live pass reads it, or it is imported/presented).
		// We iterate to a fixed point.

		bool changed = true;
		while (changed) 
		{
			changed = false;
			for (auto& PassData : RenderPassData) 
			{
				if (PassData.bCulled) 
					continue;

				bool bHasLiveOutput = false;
				for (STextureHandle Handle : PassData.Writes)
				{
					if (TextureResources[Handle.ID].RefCount > 0)
					{ 
						bHasLiveOutput = true;
						break;
					}
				}
				for (SBufferHandle Handle : PassData.BufferWrites)
				{
					if (BufferResources[Handle.ID].RefCount > 0)
					{ 
						bHasLiveOutput = true;
						break; 
					}
				}

				if (!bHasLiveOutput) 
				{
					PassData.bCulled = true;
					changed = true;

					// Decrement refcounts of everything this pass was reading
					for (STextureHandle Handle : PassData.Reads)
					{
						if (TextureResources[Handle.ID].RefCount > 0)
						{
							TextureResources[Handle.ID].RefCount--;
						}
					}

					for (SBufferHandle Handle : PassData.BufferReads)
					{
						if (BufferResources[Handle.ID].RefCount > 0)
						{
							BufferResources[Handle.ID].RefCount--;
						}
					}
				}
			}
		}
	}

	void SRenderGraph::CreatePhysicalResources()
	{
		for (STextureResource& TextureResource : TextureResources)
		{
			if (TextureResource.bIsImported)
			{
				continue;
			}

			if (bgfx::isValid(TextureResource.Handle))
			{
				continue;  // already allocated
			}

			uint16 Width = TextureResource.Description.Width;
			uint16 Height = TextureResource.Description.Height;

			if (TextureResource.Description.Scale == STextureScale::Relative) 
			{
				Width = static_cast<uint16_t>(BBWidth * TextureResource.Description.RelWidth);
				Height = static_cast<uint16_t>(BBHeight * TextureResource.Description.RelHeight);
				// Clamp to at least 1 pixel
				Width = std::max<uint16_t>(Width, 1);
				Height = std::max<uint16_t>(Height, 1);
			}

			TextureResource.Handle = bgfx::createTexture2D(
				Width, 
				Height,
				TextureResource.Description.Mips > 1,
				1,
				TextureResource.Description.Format,
				TextureResource.Description.Flags
			);

			// Create a frame buffer wrapping this texture so we can render into it
			if (TextureResource.Description.Flags & BGFX_TEXTURE_RT)
			{
				bgfx::Attachment Attachment;
				Attachment.init(TextureResource.Handle);
				TextureResource.FBHandle = bgfx::createFrameBuffer(1, &Attachment, true);
			}
		}

		// (Buffer allocation omitted for brevity — extend as needed)

	}

	void SRenderGraph::DestroyResources()
	{
		for (STextureResource& res : TextureResources) 
		{
			if (res.bIsImported)
			{
				continue;
			}

			if (bgfx::isValid(res.FBHandle)) 
			{ 
				bgfx::destroy(res.FBHandle);
				res.FBHandle = BGFX_INVALID_HANDLE;
			}

			if (bgfx::isValid(res.Handle)) 
			{ 
				bgfx::destroy(res.Handle);
				res.Handle = BGFX_INVALID_HANDLE;
			}
		}

		for (SBufferResource& res : BufferResources) 
		{
			if (res.bIsImported)
			{
				continue;
			}

			if (bgfx::isValid(res.Handle))
			{ 
				bgfx::destroy(res.Handle);
				res.Handle = BGFX_INVALID_HANDLE;
			}
		}

	}

}