#include "EditorAssetWindow.h"

#include "Engine/Engine.h"
#include "Object/World.h"
#include "Render/RenderDevice.h"

#include "IMGUI/imgui.h"

#include <algorithm>

using namespace Solus;

namespace Editor
{
	void EditorAssetWindow::Initialize()
	{
		windowFlags = ImGuiWindowFlags_NoCollapse;
	}

	void EditorAssetWindow::Render()
	{
		auto* renderDevice = gEngine->GetRenderDevice();
		if (ImGui::Begin("Assets", nullptr, windowFlags))
		{

		}
		ImGui::End();
	}

	void EditorAssetWindow::OnFocusReceived()
	{}

	void EditorAssetWindow::OnFocusLost()
	{}

	void EditorAssetWindow::OnMinimized()
	{}

	void EditorAssetWindow::OnMaximized()
	{}
}
