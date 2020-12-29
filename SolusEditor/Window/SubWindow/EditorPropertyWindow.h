#pragma once

#include "SubWindow.h"

#include "Object/Entity.h"

namespace Solus
{

	class EditorPropertyWindow : public SubWindow
	{
	public:
		// Inherited via SubWindow -->
		void Initialize() override;
		void Render() override;
		void OnFocusReceived() override;
		void OnFocusLost() override;
		void OnMinimized() override;
		void OnMaximized() override;
		// <--

		void SetEntity(Solus::Entity* entity);

	private:
		void ShowPropertyFields(const Solus::Entity* entity) const;
		void ShowComponentPropertyFields(const Solus::SComponent* component) const;

		Solus::Entity* entity;
	};

}