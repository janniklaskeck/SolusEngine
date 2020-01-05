#include "EditorPropertyWindow.h"

#include "Engine/Engine.h"

#include "Render/RenderDevice.h"

#include "IMGUI/imgui.h"

#include <algorithm>
#include <typeinfo>

#include "Render/FreeFlyCamera.h"

using namespace Solus;

namespace Editor
{
	void EditorPropertyWindow::Initialize()
	{}

	void EditorPropertyWindow::Render()
	{
		auto* renderDevice = gEngine->GetRenderDevice();
		if (ImGui::Begin("Properties") && entity)
		{
			auto* ffc = dynamic_cast<FreeFlyCamera*>(entity);
			if (ffc)
			{
				size_t offset1 = offsetof(FreeFlyCamera, asd);
				size_t offset2 = offsetof(Camera, asd2);

				auto se = sizeof(Entity);
				auto sc = sizeof(Camera);
				void* (FreeFlyCamera:: * fp)() = &FreeFlyCamera::Asd;
				//void* a = ffc->refl.call("asd", ffc);
				//bool* b = (bool*)(a);
				int c = 0;
			}
			auto id = entity->GetClassId();
			auto a = Solus::SolusObject::GetTypeInfo(id);
			ShowPropertyFields(entity, a);
		}
		ImGui::End();
	}

	void EditorPropertyWindow::OnFocusReceived()
	{}

	void EditorPropertyWindow::OnFocusLost()
	{}

	void EditorPropertyWindow::OnMinimized()
	{}

	void EditorPropertyWindow::OnMaximized()
	{}

	void EditorPropertyWindow::SetEntity(Solus::Entity* entity)
	{
		this->entity = entity;
	}

	void EditorPropertyWindow::ShowPropertyFields(Solus::Entity* entity, Solus::TypeDescriptor_Struct* metaData)
	{
		for (auto& member : metaData->members)
		{
			if (_stricmp(member.type->name, "uint64_t") == 0)
			{
				ImGui::InputInt(member.name, metaData->GetValuePtr<int>(entity, member.name));
			}
			else if (_stricmp(member.type->name, "bool") == 0)
			{
				bool* a = metaData->GetValuePtr<bool>(entity, member.name);
				ImGui::Checkbox(member.name, a);
			}
		}
		if (metaData->parent)
		{
			ShowPropertyFields(entity, metaData->parent);
		}
	}

}
