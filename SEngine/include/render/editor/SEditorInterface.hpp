#pragma once

namespace Solus
{
	struct SRenderView;

	class SEditorInterface
	{
	public:

		static void Init();

		static void Destroy();

		static void BeginInterface();

		static void RenderInterface(float DeltaTime);

		static void EndInterface();

		static void RenderEditorScene(const SRenderView& ViewRef);

		static bool IsMouseOverInterface();
	};

}