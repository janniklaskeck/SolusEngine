#pragma once
#include "Render/GLFWWindow.h"

namespace Editor
{
	class EditorSceneWindow;
	class EditorSceneGraph;
	class EditorPropertyWindow;

	class EditorMainWindow : public Solus::GLFWWindow
	{
	public:

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void Render() override;

	private:
		bool firstRun = false;
		EditorSceneWindow* sceneWindow;
		EditorSceneGraph* sceneGraph;
		EditorPropertyWindow* propertyWindow;
	};
}
