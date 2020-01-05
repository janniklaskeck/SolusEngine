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
		void ShowPropertyFields(Solus::Entity* entity, Solus::TypeDescriptor_Struct* metaData);

		Solus::Entity* entity;
	};

}