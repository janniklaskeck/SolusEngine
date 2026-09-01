#include "utils/STypes.hpp"

#include "bx/string.h"

namespace Solus
{
	SString String::Printf(const char* Format, ...)
	{
		SString Msg;

		va_list ArgList;
		va_start(ArgList, Format);
		bx::stringPrintfVargs(Msg, Format, ArgList);
		va_end(ArgList);

		return Msg;
	}

}