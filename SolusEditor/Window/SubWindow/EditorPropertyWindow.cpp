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
			auto metaData = Solus::ClassMetaData::Get(entity->GetClassId());
			ShowPropertyFields(entity, metaData);
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
			ImGui::TextColored(ImVec4(1.f, 0.f, 1.f, 1.f), "%s Member", metaData->name);
		for (auto& memberName : metaData->sortedMemberKeys)
		{
			auto* typeInfo = metaData->data[memberName];
			if (_stricmp(typeInfo->name, "uint64_t") == 0)
			{
				int* ptr = entity->Reflection.GetValuePtr<int>(memberName.c_str(), entity);
				if (ptr)
					ImGui::LabelText(memberName.c_str(), "%d", *ptr);
			}
			else if (_stricmp(typeInfo->name, "bool") == 0)
			{
				bool* ptr = entity->Reflection.GetValuePtr<bool>(memberName.c_str(), entity);
				if (ptr)
					ImGui::Checkbox(memberName.c_str(), ptr);
			} 
			else if (_stricmp(typeInfo->name, "Vec3f") == 0)
			{
				float* ptr = entity->Reflection.GetValuePtr<float>(memberName.c_str(), entity);
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
