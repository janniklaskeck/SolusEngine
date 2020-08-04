#pragma once

#include "SubWindow.h"

#include "Object/Entity.h"

namespace Editor
{

	class EditorPropertyWindow : public SubWindow
	{
	public:
		// Inherited via SubWindow -->
		virtual void Initialize() override;
		virtual void Render() override;
		virtual void OnFocusReceived() override;
		virtual void OnFocusLost() override;
		virtual void OnMinimized() override;
		virtual void OnMaximized() override;
		// <--

		void SetEntity(Solus::Entity* entity);

	private:
		void ShowPropertyFields(const Solus::Entity* entity) const;
		void ShowComponentPropertyFields(const Solus::SComponent* component) const;

		Solus::Entity* entity;
	};

}