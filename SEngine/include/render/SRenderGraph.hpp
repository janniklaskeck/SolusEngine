#pragma once
#include "bgfx/bgfx.h"

namespace Solus
{
	constexpr uint16 INVALID_ID = UINT16_MAX;

	struct STextureHandle 
	{
		uint16 ID = INVALID_ID;
		bool IsValid() const 
		{ 
			return ID != INVALID_ID;
		}

		inline bool operator==(STextureHandle Other)
		{
			return ID == Other.ID;
		}
	};

	struct SBufferHandle 
	{ 
		uint16 ID = INVALID_ID;
		bool IsValid() const 
		{
			return ID != INVALID_ID;
		}

		inline bool operator==(SBufferHandle Other)
		{
			return ID == Other.ID;
		}
	};

	// ─────────────────────────────────────────────────────────────────────────────
	//  Resource descriptors
	// ─────────────────────────────────────────────────────────────────────────────

	enum class STextureScale : uint8_t
	{
		Absolute = 0,   // width/height are pixel values
		Relative,   // width/height are fractions of the back-buffer size
	};

	struct STextureDesc 
	{
		SString Name;
		uint16 Width = 0;
		uint16 Height = 0;
		float RelWidth = 1.0f;   // used when scale == Relative
		float RelHeight = 1.0f;
		STextureScale Scale = STextureScale::Relative;
		uint8 Mips = 1;
		bgfx::TextureFormat::Enum Format = bgfx::TextureFormat::RGBA8;
		uint64 Flags = BGFX_TEXTURE_RT;
	};

	struct SBufferDesc 
	{
		SString Name;
		uint32 Size = 0;
		uint16 Flags = BGFX_BUFFER_NONE;
	};

	// ─────────────────────────────────────────────────────────────────────────────
	//  Internal resource nodes (created/managed by the graph)
	// ─────────────────────────────────────────────────────────────────────────────

	struct STextureResource
	{
		STextureDesc Description;
		bgfx::TextureHandle Handle = BGFX_INVALID_HANDLE;
		bgfx::FrameBufferHandle FBHandle = BGFX_INVALID_HANDLE;
		bool bIsImported = false;   // true → we do NOT destroy it
		int RefCount = 0;
	};

	struct SBufferResource
	{
		SBufferDesc Description;
		bgfx::DynamicVertexBufferHandle Handle = BGFX_INVALID_HANDLE;
		bool bIsImported = false;
		int RefCount = 0;
	};

	// ─────────────────────────────────────────────────────────────────────────────
	//  Pass builder  (used inside the setup lambda to declare deps)
	// ─────────────────────────────────────────────────────────────────────────────

	class SRenderGraph;

	class SPassBuilder
	{
	public:
		SPassBuilder(SRenderGraph& graph, uint32_t passIndex)
			: Graph(graph), PassIndex(passIndex)
		{
		}

		// Declare a new transient texture produced by this pass
		STextureHandle CreateTexture(const STextureDesc& desc);

		// Read an existing texture (makes it an input dependency)
		STextureHandle ReadTexture(STextureHandle handle);

		// Write to an existing texture (adds this pass as a writer)
		STextureHandle WriteTexture(STextureHandle handle);

		// Declare a new transient buffer produced by this pass
		SBufferHandle CreateBuffer(const SBufferDesc& desc);
		SBufferHandle ReadBuffer(SBufferHandle handle);
		SBufferHandle WriteBuffer(SBufferHandle handle);

		void SetView(bgfx::ViewId viewId)
		{ 
			ViewId = viewId;
		}

		bgfx::ViewId GetViewId() const
		{ 
			return ViewId;
		}

	private:
		SRenderGraph& Graph;
		uint32_t PassIndex = 0;
		bgfx::ViewId ViewId = 0;

		friend class SRenderGraph;
	};

	struct SRenderPassData 
	{
		SString Name;
		bgfx::ViewId ViewId = 0;
		bool bCulled = false;
		int RefCount = 0;   // how many other passes depend on outputs

		std::vector<STextureHandle> Reads;
		std::vector<STextureHandle> Writes;
		std::vector<SBufferHandle> BufferReads;
		std::vector<SBufferHandle> BufferWrites;

		// execute callback — receives the *compiled* graph so resources can be resolved
		std::function<void(const SRenderGraph&, bgfx::ViewId)> PassFunction;
	};

	class SRenderGraph 
	{
	public:
		SRenderGraph() = default;
		~SRenderGraph() 
		{
			DestroyResources(); 
		}

		// Non-copyable
		SRenderGraph(const SRenderGraph&) = delete;
		SRenderGraph& operator=(const SRenderGraph&) = delete;

		// ── Setup ──────────────────────────────────────────────────────────────

		/// Add a pass. Provide a setup lambda that uses the PassBuilder to declare
		/// inputs/outputs, and an execute lambda that issues bgfx draw calls.
		template<typename SetupFn, typename ExecuteFn>
		void AddPass(const SString& Name, SetupFn&& setup, ExecuteFn&& execute)
		{
			uint32 Index = static_cast<uint32_t>(RenderPassData.size());
			SRenderPassData& PassData = RenderPassData.emplace_back();
			//SRenderPassData& PassData = RenderPassData.back();
			PassData.Name = Name;

			SPassBuilder builder(*this, Index);
			setup(builder);

			PassData.ViewId = builder.GetViewId();
			PassData.PassFunction = std::forward<ExecuteFn>(execute);
		}

		/// Import an externally-owned texture (back buffer, shadow atlas, etc.)
		STextureHandle ImportTexture(const SString& Name, bgfx::TextureHandle bgfxHandle, bgfx::FrameBufferHandle fbHandle = BGFX_INVALID_HANDLE);

		/// Import an externally-owned buffer
		SBufferHandle ImportBuffer(const SString& name, bgfx::DynamicVertexBufferHandle bgfxHandle);

		// ── Compilation & execution ────────────────────────────────────────────

		/// Analyse the graph, cull dead passes, compute resource lifetimes.
		void Compile(uint16_t backbufferWidth, uint16_t backbufferHeight);

		/// Allocate physical resources and submit all passes to bgfx.
		void Execute();

		/// Reset the graph for the next frame (keeps imported resources registered).
		void Reset();

		// ── Resource resolution (called from execute lambdas) ──────────────────

		bgfx::TextureHandle GetTexture(STextureHandle Handle) const;
		bgfx::FrameBufferHandle GetFramebuffer(STextureHandle Handle) const;
		bgfx::DynamicVertexBufferHandle GetBuffer(SBufferHandle Handle) const;

		const std::vector<SRenderPassData>& GetPasses() const 
		{ 
			return RenderPassData; 
		}

		uint16 GetBackbufferWidth() const
		{
			return BBWidth; 
		}

		uint16 GetBackbufferHeight() const 
		{ 
			return BBHeight; 
		}

	private:
		friend class SPassBuilder;

		// Internal helpers
		STextureHandle AllocTextureHandle();
		SBufferHandle  AllocBufferHandle();
		void CullPasses();
		void CreatePhysicalResources();
		void DestroyResources();

		uint16 BBWidth = 0;
		uint16 BBHeight = 0;

		std::vector<SRenderPassData> RenderPassData;
		std::vector<STextureResource> TextureResources;
		std::vector<SBufferResource> BufferResources;

		bool bCompiled = false;
	};

}