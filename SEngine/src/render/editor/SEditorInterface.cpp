#include "render/editor/SEditorInterface.hpp"

#include "render/imgui/imgui_bgfx.hpp"
#include "core/processor/SInputProcessor.hpp"
#include "../../../include/render/SWindow.hpp"
#include "../../../include/core/processor/SRenderingProcessor.hpp"
#include "../../../../extern/fc_cache/imgui-src/imgui_internal.h"

namespace Solus
{



	void SEditorInterface::Init()
	{
		imguiCreate();
	}

	void SEditorInterface::Destroy()
	{
		imguiDestroy();
	}

	void SEditorInterface::BeginInterface()
	{
		const SInputProcessor& InputProcessor = gEngine->GetProcessor<SInputProcessor>();
		const Vec2 MousePos = InputProcessor.GetMousePos();
		const int32 MouseWheelDelta = InputProcessor.GetMouseWheelDelta();

		SWindow* Window = gEngine->GetWindow();

		imguiBeginFrame(
			MousePos.x,
			MousePos.y,
			InputProcessor.IsMouseButtonDown(SMouseButtonCode::LeftButton) ? IMGUI_MBUT_LEFT : 0 |
			InputProcessor.IsMouseButtonDown(SMouseButtonCode::RightButton) ? IMGUI_MBUT_RIGHT : 0 |
			InputProcessor.IsMouseButtonDown(SMouseButtonCode::MiddleButton) ? IMGUI_MBUT_MIDDLE : 0,
			MouseWheelDelta,
			Window->GetWindowWidth(),
			Window->GetWindowHeight());
	}

	void SEditorInterface::RenderInterface(float DeltaTime)
	{
		static bool first_time = true;
		ImGuiID EditorDockSpaceID = ImGui::GetID("EditorDockSpace");
		if (first_time)
		{
			first_time = false;
			// Clear any existing layout
			ImGui::DockBuilderRemoveNode(EditorDockSpaceID);
			ImGui::DockBuilderAddNode(EditorDockSpaceID, ImGuiDockNodeFlags_DockSpace | ImGuiDockNodeFlags_NoUndocking);
			ImGui::DockBuilderSetNodeSize(EditorDockSpaceID, ImGui::GetIO().DisplaySize);

			ImGuiID TopDockID = ImGui::DockBuilderSplitNode(EditorDockSpaceID, ImGuiDir_Up, 0.1f, nullptr, nullptr);
			ImGuiID RightDockID = ImGui::DockBuilderSplitNode(EditorDockSpaceID, ImGuiDir_Right, 0.2f, nullptr, nullptr);
			ImGuiID DownDockID = ImGui::DockBuilderSplitNode(EditorDockSpaceID, ImGuiDir_Down, 0.25f, nullptr, nullptr);
			ImGuiID CenterDockID = ImGui::DockBuilderGetCentralNode(EditorDockSpaceID)->ID;

			// Dock windows into specific nodes
			ImGui::DockBuilderDockWindow("EditorToolBar", TopDockID);
			ImGui::DockBuilderDockWindow("EditorWorldAndDetails", RightDockID);
			ImGui::DockBuilderDockWindow("EditorAssetView", DownDockID);
			ImGui::DockBuilderDockWindow("EditorViewport", CenterDockID);

			ImGui::DockBuilderFinish(EditorDockSpaceID);
		}

		ImGuiWindowClass DockWindowClass{};
		DockWindowClass.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoUndocking;

		ImGuiViewport* viewport = ImGui::GetMainViewport();

		ImGui::SetNextWindowPos({});
		ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
		ImGui::SetNextWindowViewport(ImGui::GetMainViewport()->ID);
		
		ImGuiWindowFlags flags =
			ImGuiWindowFlags_NoTitleBar |
			ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoBringToFrontOnFocus |
			ImGuiWindowFlags_NoNavFocus;

		ImGui::Begin("FullscreenDockspace", nullptr, flags);


		ImGui::DockSpace(EditorDockSpaceID);

		ImGui::SetNextWindowClass(&DockWindowClass);
		ImGuiWindowFlags EditorWindowFlags = ImGuiWindowFlags_::ImGuiWindowFlags_NoResize | ImGuiWindowFlags_::ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_::ImGuiWindowFlags_NoMove;
		ImGui::Begin("EditorToolBar", nullptr, EditorWindowFlags | ImGuiWindowFlags_MenuBar);

		ImGui::BeginMenuBar();
		
		if (ImGui::BeginMenu("File"))
		{
			ImGui::MenuItem("(demo menu)", NULL, false, false);
			if (ImGui::MenuItem("New")) {}
			if (ImGui::MenuItem("Open", "Ctrl+O")) {}
			if (ImGui::BeginMenu("Open Recent"))
			{
				ImGui::MenuItem("fish_hat.c");
				ImGui::MenuItem("fish_hat.inl");
				ImGui::MenuItem("fish_hat.h");
				if (ImGui::BeginMenu("More.."))
				{
					ImGui::MenuItem("Hello");
					ImGui::MenuItem("Sailor");
					if (ImGui::BeginMenu("Recurse.."))
					{
						//ShowExampleMenuFile();
						ImGui::EndMenu();
					}
					ImGui::EndMenu();
				}
				ImGui::EndMenu();
			}
			if (ImGui::MenuItem("Save", "Ctrl+S")) {}
			if (ImGui::MenuItem("Save As..")) {}

			ImGui::Separator();
			if (ImGui::BeginMenu("Options"))
			{
				static bool enabled = true;
				ImGui::MenuItem("Enabled", "", &enabled);
				ImGui::BeginChild("child", ImVec2(0, ImGui::GetTextLineHeightWithSpacing() * 5.0f), ImGuiChildFlags_Borders);
				for (int i = 0; i < 10; i++)
					ImGui::Text("Scrolling Text %d", i);
				ImGui::EndChild();
				static float f = 0.5f;
				static int n = 0;
				ImGui::SliderFloat("Value", &f, 0.0f, 1.0f);
				ImGui::InputFloat("Input", &f, 0.1f);
				ImGui::Combo("Combo", &n, "Yes\0No\0Maybe\0\0");
				ImGui::EndMenu();
			}

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit"))
		{
			if (ImGui::MenuItem("Undo", "Ctrl+Z")) {}
			if (ImGui::MenuItem("Redo", "Ctrl+Y", false, false)) {} // Disabled item
			ImGui::Separator();
			if (ImGui::MenuItem("Cut", "Ctrl+X")) {}
			if (ImGui::MenuItem("Copy", "Ctrl+C")) {}
			if (ImGui::MenuItem("Paste", "Ctrl+V")) {}
			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();

		ImGui::Button("Test");

		ImGui::End();

		ImGui::SetNextWindowClass(&DockWindowClass);
		ImGui::Begin("EditorWorldAndDetails", nullptr, EditorWindowFlags);

		ImGui::End();

		ImGui::SetNextWindowClass(&DockWindowClass);
		ImGui::Begin("EditorAssetView", nullptr, EditorWindowFlags);

		ImGui::End();


		ImGui::End();
	}

	void SEditorInterface::EndInterface()
	{
		imguiEndFrame();
	}

	void SEditorInterface::RenderEditorScene(const SRenderView& ViewRef)
	{
		bgfx::touch(ViewRef.ViewId);

		ImGui::Begin("EditorViewport"); // this window can be docked freely

		ImVec2 avail = ImGui::GetContentRegionAvail();
		uint16_t w = (uint16_t)avail.x;
		uint16_t h = (uint16_t)avail.y;

		// Resize the framebuffer if the panel size changed (and is valid)
		if (w > 0 && h > 0 && (w != ViewRef.Width || h != ViewRef.Height))
		{
			//createSceneFramebuffer(sv, w, h);
		}

		if (bgfx::isValid(ViewRef.ViewTexture))
		{
			ImTextureID TextureID = (ImTextureID)(uintptr_t)ViewRef.ViewTexture.idx;
			ImGui::Image(TextureID, ImVec2((float)ViewRef.Width, (float)ViewRef.Height));
		}

		ImGui::End();
	}

	bool SEditorInterface::IsMouseOverInterface()
	{
		return ImGui::MouseOverArea();
	}

}