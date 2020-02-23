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
	{
		windowFlags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_MenuBar;
	}

	void EditorPropertyWindow::Render()
	{
		auto size = gEngine->GetWindow()->GetWindowSize();
		ImGui::SetNextWindowPos(ImVec2(size.x * WINDOW_PROPERTY_POS_X, size.y * WINDOW_PROPERTY_POS_Y));
		ImGui::SetNextWindowSize(ImVec2(size.x * WINDOW_PROPERTY_SIZE_X, size.y * WINDOW_PROPERTY_SIZE_Y));
		if (ImGui::Begin("Properties", nullptr, windowFlags))
		{
			if (entity)
			{
				auto metaData = Solus::ClassMetaData::Get(entity->GetClassId());
				ShowPropertyFields(entity, metaData);
			}
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
		if (!entity || !metaData)
			return;
		if (metaData->data.size() > 0)
			ImGui::TextColored(ImVec4(1.f, 0.f, 1.f, 1.f), "%s", metaData->name);
		for (auto& memberName : metaData->sortedMemberKeys)
		{
			auto* typeInfo = metaData->data[memberName];
			if (typeInfo->IsType("uint64_t"))
			{
				int* ptr = Solus::ClassMetaData::GetValuePtr<int>(memberName.c_str(), entity);
				if (ptr)
				{
					if (_stricmp(memberName.c_str(), "entityId") == 0)
						ImGui::LabelText(memberName.c_str(), "%d", *ptr);
					else
						ImGui::InputInt(memberName.c_str(), ptr);
				}
			}
			else if (typeInfo->IsType("bool"))
			{
				bool* ptr = Solus::ClassMetaData::GetValuePtr<bool>(memberName.c_str(), entity);
				if (ptr)
					ImGui::Checkbox(memberName.c_str(), ptr);
			}
			else if (typeInfo->IsType("float"))
			{
				float* ptr = Solus::ClassMetaData::GetValuePtr<float>(memberName.c_str(), entity);
				if (ptr)
					ImGui::InputFloat(memberName.c_str(), ptr);
			}
			else if (typeInfo->IsType("Vec3f"))
			{
				float* ptr = Solus::ClassMetaData::GetValuePtr<float>(memberName.c_str(), entity);
				if (ptr)
					ImGui::InputFloat3(memberName.c_str(), ptr);
			}
		}
		if (metaData->parents.size())
		{
			for (auto* parent : metaData->parents)
				ShowPropertyFields(entity, parent);
		}
	}

}
