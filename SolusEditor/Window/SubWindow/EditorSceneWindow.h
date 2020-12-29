#pragma once

#include "SubWindow.h"

#include "Render/FreeFlyCamera.h"

namespace Solus
{

	class EditorSceneWindow : public SubWindow
	{
	public:
		// Inherited via SubWindow -->
		void Initialize() override;
		void Render() override;
		void OnFocusReceived() override;
		void OnFocusLost() override;
		void OnMinimized() override;
		void OnMaximized() override;
		// <--

	private:
		Solus::FreeFlyCamera* sceneCamera;
	};

}