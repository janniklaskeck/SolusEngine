#include "EditorMainWindow.h"
#include "EditorInputDevice.h"

#include "Input/InputDevice.h"

#include "Engine/Engine.h"

#include "Render/FreeFlyCamera.h"
#include "Render/RenderDevice.h"
#include "Render/OpenGLSurface.h"
#include "Render/GLFWWindow.h"

#include "Object/World.h"

#include "AssetSystem/AssetManager.h"
#include "AssetSystem/FolderAssetSource.h"
#include "AssetSystem/Asset.h"

#include "IMGUI/imgui.h"
#include "IMGUI/imgui_impl_glfw.h"
#include "IMGUI/imgui_impl_opengl3.h"

using namespace Solus;

namespace Editor
{
	void EditorMainWindow::Initialize()
	{
		GLFWWindow::Initialize();
		isEditorWindow = true;
		gEngine->RegisterInputDevice(new EditorInputDevice);
		if (gl3wInit())
		{
			fprintf(stderr, "failed to initialize OpenGL\n");
		}
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 460 core");

		ImGui::StyleColorsDark();

	}

	void EditorMainWindow::Update()
	{
		if (!firstRun)
		{
			gEngine->GetRenderDevice()->SetShouldRenderScene(false);
			FreeFlyCamera* camera = gEngine->GetWorld()->SpawnEntity<FreeFlyCamera>(Vec3f(0, 0, -5), Vec3f(0, 0, 0));
			auto* entity = gEngine->GetWorld()->SpawnEntity<Entity>(Vec3f(0, 0, 0), Vec3f(0, 0, 0));
			entity->TEMP();
			firstRun = true;
		}
	}

	void EditorMainWindow::Render()
	{
		auto* renderDevice = gEngine->GetRenderDevice();
		renderDevice->PreRenderScene();

		gEngine->GetWorld()->Render();
		renderDevice->PostRenderScene();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();

		PreRenderUI();
		ImGui::NewFrame();

		//ImGui::ShowDemoWindow();
		auto texture = ((OpenGLSurface*)renderDevice->GetCurrentRenderSurface())->GetColorTexture();
		ImGui::Begin("Scene Window");

		ImVec2 pos = ImGui::GetCursorScreenPos();

		ImVec2 windowTopLeft = ImGui::GetCursorScreenPos();
		int width, height;
		GetWindowSize(&width, &height);
		ImVec2 windowBottomRight = ImVec2(windowTopLeft.x + width / 2, windowTopLeft.y + height / 2);
		ImGui::GetWindowDrawList()->AddImage((void*)texture, windowTopLeft, windowBottomRight, ImVec2(0, 1), ImVec2(1, 0));

		ImGui::End();
		RenderUI();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	}
}
