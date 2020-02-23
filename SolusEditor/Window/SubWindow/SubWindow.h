#pragma once

namespace Editor
{
	const float WINDOW_SCENE_SIZE_X = .7f;
	const float WINDOW_SCENE_SIZE_Y = .7f;

	const float WINDOW_ASSET_POS_X = .0f;
	const float WINDOW_ASSET_POS_Y = .7f;

	const float WINDOW_ASSET_SIZE_X = .7f;
	const float WINDOW_ASSET_SIZE_Y = .3f;

	const float WINDOW_PROPERTY_POS_X = .7f;
	const float WINDOW_PROPERTY_POS_Y = .5f;

	const float WINDOW_PROPERTY_SIZE_X = .3f;
	const float WINDOW_PROPERTY_SIZE_Y = .5f;

	const float WINDOW_GRAPH_POS_X = .7f;
	const float WINDOW_GRAPH_POS_Y = .0f;

	const float WINDOW_GRAPH_SIZE_X = .3f;
	const float WINDOW_GRAPH_SIZE_Y = .5f;

	class SubWindow
	{
	public:
		virtual void Initialize() = 0;
		virtual void Render() = 0;

		virtual void OnFocusReceived() = 0;
		virtual void OnFocusLost() = 0;

		virtual void OnMinimized() = 0;
		virtual void OnMaximized() = 0;

	protected:
		unsigned int windowFlags = 0;
	};
}