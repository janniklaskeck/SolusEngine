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
			auto a = Solus::ClassMetaData::Get(id);
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

	void EditorPropertyWindow::ShowPropertyFields(Solus::Entity* entity, Solus::ClassMetaData* metaData)
	{
		for (auto it = metaData->data.begin(); it != metaData->data.end(); it++)
		{
			auto& member = *it;
			if (_stricmp(member.second->name, "uint64_t") == 0)
			{
				int* ptr = entity->Reflection.GetValuePtr<int>(member.first.c_str(), entity);
				if (ptr)
					ImGui::LabelText(member.first.c_str(), "%d", *ptr);
			}
			else if (_stricmp(member.second->name, "bool") == 0)
			{
				bool* ptr = entity->Reflection.GetValuePtr<bool>(member.first.c_str(), entity);
				if (ptr)
					ImGui::Checkbox(member.first.c_str(), ptr);
			} 
			else if (_stricmp(member.second->name, "Vec3f") == 0)
			{
				float* ptr = entity->Reflection.GetValuePtr<float>(member.first.c_str(), entity);
				if (ptr)
					ImGui::InputFloat3(member.first.c_str(), ptr);
			}
		}
		if (metaData->parent)
		{
			ShowPropertyFields(entity, metaData->parent);
		}
	}

}
