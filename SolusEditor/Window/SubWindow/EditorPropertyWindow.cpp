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
				int* ptr = entity->Reflection_Acc.GetValuePtr<int>(member.name, entity);
				if (ptr)
					ImGui::LabelText(member.name, "%d", *ptr);
			}
			else if (_stricmp(member.type->name, "bool") == 0)
			{
				bool* ptr = entity->Reflection_Acc.GetValuePtr<bool>(member.name, entity);
				if (ptr)
					ImGui::Checkbox(member.name, ptr);
			} 
			else if (_stricmp(member.type->name, "Vec3f") == 0)
			{
				float* ptr = entity->Reflection_Acc.GetValuePtr<float>(member.name, entity);
				if (ptr)
					ImGui::InputFloat3(member.name, ptr);
			}
		}
		if (metaData->parent)
		{
			ShowPropertyFields(entity, metaData->parent);
		}
	}

}
