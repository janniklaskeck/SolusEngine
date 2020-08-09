#pragma once
#include "Render/GLFWWindow.h"
#include "SubWindow/SubWindow.h"
#include "SubWindow/EditorSceneWindow.h"
#include "SubWindow/EditorSceneGraph.h"
#include "SubWindow/EditorPropertyWindow.h"
#include "SubWindow/EditorAssetWindow.h"
#include "SubWindow/EditorLogWindow.h"
#include "SubWindow/EditorMenuBar.h"

#include <vector>
#include <memory>

namespace Editor
{
	class EditorSceneWindow;
	class EditorSceneGraph;
	class EditorPropertyWindow;
	class EditorAssetWindow;
	class EditorLogWindow;
	class EditorMenuBar;

	class EditorMainWindow : public Solus::GLFWWindow
	{
	public:

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void Render() override;
		virtual void Destroy() override;

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

	};
}
