#pragma once
#include "Render/GLFWWindow.h"
#include "SubWindow/SubWindow.h"

#include <vector>

namespace Editor
{
	class EditorSceneWindow;
	class EditorSceneGraph;
	class EditorPropertyWindow;
	class EditorAssetWindow;
	class EditorLogWindow;

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
		bool firstRun = false;
		EditorSceneWindow* sceneWindow;
		EditorSceneGraph* sceneGraph;
		EditorPropertyWindow* propertyWindow;
		EditorAssetWindow* assetWindow;
		EditorLogWindow* logWindow;

		std::vector<SubWindow*> subWindows;

	};
}
