#include "EditorMainWindow.h"
#include "EditorInputDevice.h"

#include "SubWindow/EditorSceneWindow.h"
#include "SubWindow/EditorSceneGraph.h"
#include "SubWindow/EditorPropertyWindow.h"
#include "SubWindow/EditorAssetWindow.h"
#include "SubWindow/EditorLogWindow.h"
#include "SubWindow/EditorMenuBar.h"

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

#include "Utility/Timer.h"

#include "IMGUI/imgui.h"
#include "IMGUI/imgui_impl_glfw.h"
#include "IMGUI/imgui_impl_opengl3.h"

namespace Solus
{

	EditorMainWindow::EditorMainWindow()
	{}

	EditorMainWindow::~EditorMainWindow()
	{}

	EditorMainWindow* EditorMainWindow::Instance()
	{
		return static_cast<EditorMainWindow*>(gEngine->GetWindow());
	}

	void EditorMainWindow::Initialize()
	{
		width = 1600;
		height = 900;
		title = "SolusEngine Editor";
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

		editorTimer = std::make_unique<Timer>();
		editorTimer->Initialize();

		menuBar = std::make_unique<EditorMenuBar>();
		sceneWindow = std::make_unique<EditorSceneWindow>();
		sceneGraph = std::make_unique<EditorSceneGraph>();
		propertyWindow = std::make_unique<EditorPropertyWindow>();
		assetWindow = std::make_unique<EditorAssetWindow>();
		logWindow = std::make_unique<EditorLogWindow>();

		subWindows.push_back(menuBar.get());
		subWindows.push_back(sceneWindow.get());
		subWindows.push_back(sceneGraph.get());
		subWindows.push_back(propertyWindow.get());
		subWindows.push_back(assetWindow.get());
		subWindows.push_back(logWindow.get());

		for (auto* subWindow : subWindows)
		{
			subWindow->Initialize();
		}

		windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

		SetWindowTitle("Editor - " + gEngine->GetCurrentProject()->GetProjectName());
	}

	void EditorMainWindow::Update()
	{
		gEngine->GetRenderDevice()->SetShouldRenderScene(false);

		editorTimer->Update();
		const float deltaTime = (float)editorTimer->GetDeltaTime();
		for (auto* subWindow : subWindows)
		{
			subWindow->Update(deltaTime);
		}

		propertyWindow->SetEntity(sceneGraph->GetSelectedEntity());
	}

	void EditorMainWindow::Render()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();

		PreRenderUI();
		ImGui::NewFrame();

		RenderDockspace();

		ImGui::ShowDemoWindow();

		for (auto* subWindow : subWindows)
		{
			subWindow->Render();
		}

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

	double EditorMainWindow::DeltaTime() const
	{
		return editorTimer->GetDeltaTime();
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

	const Timer* EditorMainWindow::GetEditorTimer() const
	{
		return editorTimer.get();
	}
}
