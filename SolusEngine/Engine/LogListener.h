#pragma once

#include "Engine.h"

namespace Solus
{
	class LogListener
	{
	public:
		virtual void OnLogReceived(LogLevel level, const char* message) = 0;
	};
}