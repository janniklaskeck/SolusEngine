#pragma once

#include "SDL3/SDL_gpu.h"

namespace Solus
{
	struct SRenderPassCommand
	{
		SDL_GPUColorTargetInfo ColorTargetInfo{};

	public:

		static SRenderPassCommand GetClearCommand(const SFColor ClearColor)
		{
			SRenderPassCommand Command;
			Command.ColorTargetInfo.load_op = SDL_GPU_LOADOP_CLEAR;
			Command.ColorTargetInfo.store_op = SDL_GPU_STOREOP_STORE;
			Command.ColorTargetInfo.clear_color = ClearColor;

			return Command;
		}
	};
}
