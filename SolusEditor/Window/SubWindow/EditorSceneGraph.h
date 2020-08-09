#pragma once

#include "SubWindow.h"

#include "Object/Entity.h"

namespace Editor
{

	class EditorSceneGraph : public SubWindow
	{
	public:
		// Inherited via SubWindow -->
		virtual void Initialize() override;
		virtual void Update(float deltaTime) override;
		virtual void Render() override;
		virtual void OnFocusReceived() override;
		virtual void OnFocusLost() override;
		virtual void OnMinimized() override;
		virtual void OnMaximized() override;
		// <--

		Solus::Entity* GetSelectedEntity() const;
	private:
		Solus::Entity* selectedEntity = nullptr;
	};

}