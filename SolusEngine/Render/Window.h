#pragma once

#include "Engine/SolusEngine.h"

namespace Solus
{
	class SOLUS_API Window : public SubSystem
	{
	public:

		virtual void Initialize() = 0;
		virtual void Run() = 0;
		virtual void Update() = 0;
		virtual void Render() = 0;
		virtual void Destroy() = 0;

		virtual void PreRenderUI() = 0;
		virtual void RenderUI() = 0;

		virtual void GetCursorPos(double* xPos, double* yPos) = 0;
		virtual double GetTime() const = 0;
		virtual void GetWindowSize(int* xSize, int* ySize) = 0;

		bool IsEditorWindow() const
		{
			return isEditorWindow;
		}
	protected:
		bool isEditorWindow = false;
	};
}
