#pragma once

namespace Solus
{

	class SEditorInterface
	{
	public:

		static void Init();

		static void Destroy();

		static void RenderInterface(float DeltaTime);
	};

}