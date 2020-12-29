#pragma once

#include "SubWindow.h"

#include "Object/Entity.h"

namespace Solus
{

	class EditorSceneGraph : public SubWindow
	{
	public:
		// Inherited via SubWindow -->
		void Initialize() override;
		void Update(float deltaTime) override;
		void Render() override;
		void OnFocusReceived() override;
		void OnFocusLost() override;
		void OnMinimized() override;
		void OnMaximized() override;
		// <--

		Solus::Entity* GetSelectedEntity() const;
	private:
		Solus::Entity* selectedEntity = nullptr;
	};

}