#pragma once
#include "Render/GLFWWindow.h"

namespace Editor
{
	class EditorSceneWindow;
	class EditorSceneGraph;
	class EditorPropertyWindow;
	class EditorAssetWindow;

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
		bool firstRun = false;
		EditorSceneWindow* sceneWindow;
		EditorSceneGraph* sceneGraph;
		EditorPropertyWindow* propertyWindow;
		EditorAssetWindow* assetWindow;

		int windowFlags;
	};
}
