#pragma once

#include "SubWindow.h"

#include "Render/FreeFlyCamera.h"

namespace Solus
{

	class EditorSceneWindow : public SubWindow
	{
	public:
		// Inherited via SubWindow -->
		virtual void Initialize() override;
		virtual void Render() override;
		virtual void OnFocusReceived() override;
		virtual void OnFocusLost() override;
		virtual void OnMinimized() override;
		virtual void OnMaximized() override;
		// <--

	private:
		Solus::FreeFlyCamera* sceneCamera;
	};

}