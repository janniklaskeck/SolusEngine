#include "EditorMainWindow.h"
#include "EditorInputDevice.h"

#include "SubWindow/EditorSceneWindow.h"
#include "SubWindow/EditorSceneGraph.h"
#include "SubWindow/EditorPropertyWindow.h"
#include "SubWindow/EditorAssetWindow.h"

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
		ImGuiIO& io = ImGui::GetIO();
		(void)io;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

		ImGui::StyleColorsDark();

		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 460 core");

		sceneWindow = new EditorSceneWindow;
		sceneWindow->Initialize();

		sceneGraph = new EditorSceneGraph;
		sceneGraph->Initialize();

		propertyWindow = new EditorPropertyWindow;
		propertyWindow->Initialize();

		assetWindow = new EditorAssetWindow;
		assetWindow->Initialize();

		windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	}

	void EditorMainWindow::Update()
	{
		if (!firstRun)
		{
			gEngine->GetRenderDevice()->SetShouldRenderScene(false);
			
			auto* entity = gEngine->GetWorld()->SpawnEntity<Entity>(Vec3f(0, 0, 0), Vec3f(0, 0, 0));
			entity->TEMP();

			entity = gEngine->GetWorld()->SpawnEntity<Entity>(Vec3f(5, 2, 0), Vec3f(0, 0, 0));
			entity->TEMP();

			firstRun = true;
		}

		propertyWindow->SetEntity(sceneGraph->GetSelectedEntity());
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

		RenderDockspace();

		ImGui::ShowDemoWindow();

		sceneWindow->Render();
		sceneGraph->Render();
		propertyWindow->Render();
		assetWindow->Render();

		RenderUI();
		ImGui::End();
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
	}
	
	void EditorMainWindow::Destroy()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
		GLFWWindow::Destroy();

	}
	
	void EditorMainWindow::RenderDockspace()
	{
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("SolusEngine", nullptr, windowFlags);
		ImGui::PopStyleVar(3);

		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
		ImGuiID mainDockspaceId = ImGui::GetID("MainWindowDock");
		ImGui::DockSpace(mainDockspaceId, ImVec2(0.f, 0.f));
	}
}
