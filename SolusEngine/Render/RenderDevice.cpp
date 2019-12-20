#include "RenderDevice.h"

namespace Solus
{
	void RenderDevice::SetShouldRenderScene(bool shouldRender)
	{
		shouldRenderScene = shouldRender;
	}
	bool RenderDevice::ShouldRenderScene() const
	{
		return shouldRenderScene;
	}
}
