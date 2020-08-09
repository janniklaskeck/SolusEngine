#pragma once

#include "Engine/SolusEngine.h"
#include "Utility/Vector.h"

namespace Solus
{
	class SOLUS_API Window : public SubSystem
	{
	public:

		virtual void Initialize();
		virtual void Run() = 0;
		virtual void Update() = 0;
		virtual void Render() = 0;
		virtual void Destroy() = 0;

		virtual void PreRenderUI() = 0;
		virtual void RenderUI() = 0;

		virtual void GetCursorPos(double* xPos, double* yPos) = 0;
		virtual double GetTime() const = 0;

		virtual Vec2i GetWindowPosition() const = 0;
		virtual Vec2i GetWindowSize() const = 0;

		virtual void SetMouseVisible(bool isVisible) = 0;

		virtual void SetWindowTitle(const std::string title) = 0;

		bool IsEditorWindow() const
		{
			return isEditorWindow;
		}
	protected:
		bool isEditorWindow = false;
	};
}
