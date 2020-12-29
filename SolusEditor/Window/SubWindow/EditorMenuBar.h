#pragma once

#include "SubWindow.h"

namespace Solus
{

	class EditorMenuBar : public SubWindow
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
	};

}