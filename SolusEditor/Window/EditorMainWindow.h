#pragma once
#include "Render/GLFWWindow.h"
#include "SubWindow/SubWindow.h"

#include <vector>
#include <memory>

namespace Solus
{
	class EditorSceneWindow;
	class EditorSceneGraph;
	class EditorPropertyWindow;
	class EditorAssetWindow;
	class EditorLogWindow;
	class EditorMenuBar;
	class Timer;

	class EditorMainWindow : public Solus::GLFWWindow
	{
	public:
		EditorMainWindow();
		~EditorMainWindow();

		static EditorMainWindow* Instance();

		void Initialize() override;
		void Update() override;
		void Render() override;
		void Destroy() override;

		double DeltaTime() const;

	private:
		void RenderDockspace();
	private:
		int windowFlags;
		std::unique_ptr<EditorSceneWindow> sceneWindow;
		std::unique_ptr<EditorSceneGraph> sceneGraph;
		std::unique_ptr<EditorPropertyWindow> propertyWindow;
		std::unique_ptr<EditorAssetWindow> assetWindow;
		std::unique_ptr<EditorLogWindow> logWindow;
		std::unique_ptr<EditorMenuBar> menuBar;

		std::vector<SubWindow*> subWindows;

		const Timer* GetEditorTimer() const;
		std::unique_ptr<Timer> editorTimer;

	};
}
