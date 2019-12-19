#pragma once

#include "Engine/SolusEngine.h"

namespace Solus
{
	class SOLUS_API RenderWindow
	{
	public:
		virtual void Initialize() = 0;
		virtual void Run() = 0;
		virtual void Update() = 0;
		virtual void Close() = 0;

		virtual void GetCursorPos(double* xPos, double* yPos) = 0;

		virtual double GetTime() const = 0;

		virtual void GetWindowSize(int* xSize, int* ySize) = 0;
	};
}

