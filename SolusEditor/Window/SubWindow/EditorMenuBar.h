#pragma once

#include "SubWindow.h"

namespace Editor
{

	class EditorMenuBar : public SubWindow
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
	};

}