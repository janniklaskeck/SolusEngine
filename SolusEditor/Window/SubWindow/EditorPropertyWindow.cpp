#include "EditorPropertyWindow.h"

#include "Engine/Engine.h"
#include "AssetSystem/MeshAsset.h"
#include "Object/Component/MeshComponent.h"

#include "Render/RenderDevice.h"

#include "IMGUI/imgui.h"

#include <algorithm>
#include <typeinfo>

#include "Render/FreeFlyCamera.h"

namespace Solus
{

	void EditorPropertyWindow::Initialize()
	{
		windowFlags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_MenuBar;
	}

	void EditorPropertyWindow::Render()
	{
		if (ImGui::Begin("Properties", nullptr, windowFlags))
		{
			if (entity)
			{
				ShowPropertyFields(entity);
				for (const auto& component : entity->GetComponents())
				{
					ShowComponentPropertyFields(component);
				}
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

	void EditorPropertyWindow::ShowPropertyFields(const Solus::Entity* entity) const
	{
		if (!entity)
			return;
		auto entityType = entity->get_type();
		Solus::ClassMetaData* metaDataPtr = entity->GetClassMetaData();
		ImGui::TextColored(ImVec4(1.f, 0.f, 1.f, 1.f), "%s", std::string(entityType.get_name()).c_str());
		for (auto& property : entityType.get_properties())
		{
			std::string propertyName = std::string(property.get_name());
			if (property.get_type() == rttr::type::get<uint64_t>())
			{
				uint64_t* value = metaDataPtr->GetMemberPtr<uint64_t>(entity, propertyName);
				if (propertyName == "entityId")
					ImGui::LabelText(propertyName.c_str(), "%d", *value);
				else
					ImGui::InputScalar(propertyName.c_str(), ImGuiDataType_::ImGuiDataType_U64, value);
			}
			else if (property.get_type() == rttr::type::get<bool>())
			{
				bool* a = metaDataPtr->GetMemberPtr<bool>(entity, propertyName);
				ImGui::Checkbox(propertyName.c_str(), a);
			}
			else if (property.get_type() == rttr::type::get<float>())
			{
				float* value = metaDataPtr->GetMemberPtr<float>(entity, propertyName);
				ImGui::InputFloat(propertyName.c_str(), value);
			}
			else if (property.get_type() == rttr::type::get<Vec3f>())
			{
				Vec3f* value = metaDataPtr->GetMemberPtr<Vec3f>(entity, propertyName);
				ImGui::InputFloat3(propertyName.c_str(), (float*)value);
			}

		}
	}

	void EditorPropertyWindow::ShowComponentPropertyFields(const Solus::SComponent* component) const
	{
		if (!component)
			return;
		auto entityType = component->get_type();
		Solus::ClassMetaData* metaDataPtr = component->GetClassMetaData();
		if (entityType.get_properties().size() > 0)
			ImGui::TextColored(ImVec4(1.f, 0.f, 1.f, 1.f), "%s", std::string(entityType.get_name()).c_str());
		for (auto& property : entityType.get_properties())
		{
			const std::string propName = std::string(property.get_name());
			if (property.get_type() == rttr::type::get<uint64_t>())
			{
				uint64_t* ptr = metaDataPtr->GetMemberPtr<uint64_t>(component, propName);
				if (_stricmp(propName.c_str(), "entityId") == 0)
					ImGui::LabelText(propName.c_str(), "%d", *ptr);
				else
					ImGui::InputScalar(propName.c_str(), ImGuiDataType_::ImGuiDataType_U64, ptr);
			}
			else if (property.get_type() == rttr::type::get<bool>())
			{
				bool* ptr = metaDataPtr->GetMemberPtr<bool>(component, propName);
				ImGui::Checkbox(propName.c_str(), ptr);
			}
			else if (property.get_type() == rttr::type::get<float>())
			{
				float* ptr = metaDataPtr->GetMemberPtr<float>(component, propName);
				ImGui::InputFloat(propName.c_str(), ptr);
			}
			else if (property.get_type() == rttr::type::get<Vec3f>())
			{
				Vec3f* ptr = metaDataPtr->GetMemberPtr<Vec3f>(component, propName);
				ImGui::InputFloat3(propName.c_str(), (float*)ptr);
			}
			else if (property.get_type() == rttr::type::get<MeshAsset*>())
			{
				Solus::MeshAsset* ptr = *metaDataPtr->GetMemberPtr<Solus::MeshAsset*>(component, propName);
				if (ptr)
				{
					ImGui::LabelText(propName.c_str(), ptr->GetFilePath(true).string().c_str());
				}
				else
				{
					ImGui::LabelText(propName.c_str(), "No Asset...");
				}
				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("TESTDRAG"))
					{
						Solus::Asset** payload_n = (Solus::Asset**)payload->Data;
						if (payload_n)
						{
							auto* meshComp = (Solus::MeshComponent*)component;
							auto* meshAsset = (MeshAsset*)(*payload_n);
							meshComp->SetMesh(meshAsset);
						}
					}
					ImGui::EndDragDropTarget();
				}
			}
		}
	}

}
