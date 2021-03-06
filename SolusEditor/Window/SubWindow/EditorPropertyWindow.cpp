#include "EditorPropertyWindow.h"

#include "Engine/Engine.h"
#include "Object/Component/SComponent.h"

#include "Render/RenderDevice.h"

#include "AssetSystem/AssetManager.h"
#include "AssetSystem/SAsset.h"

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
		for (auto& prop : entityType.get_properties())
		{
			const auto propertyType = prop.get_type();
			auto wrappedPropertyType = propertyType.get_wrapped_type();
			auto p = wrappedPropertyType.get_wrapped_type();
			const std::string propName = std::string(prop.get_name());
			if (propertyType == rttr::type::get<uint64_t>())
			{
				uint64_t* ptr = metaDataPtr->GetMemberPtr<uint64_t>(component, propName);
				if (_stricmp(propName.c_str(), "entityId") == 0)
					ImGui::LabelText(propName.c_str(), "%d", *ptr);
				else
					ImGui::InputScalar(propName.c_str(), ImGuiDataType_::ImGuiDataType_U64, ptr);
			}
			else if (propertyType == rttr::type::get<bool>())
			{
				bool* ptr = metaDataPtr->GetMemberPtr<bool>(component, propName);
				ImGui::Checkbox(propName.c_str(), ptr);
			}
			else if (propertyType == rttr::type::get<float>())
			{
				float* ptr = metaDataPtr->GetMemberPtr<float>(component, propName);
				ImGui::InputFloat(propName.c_str(), ptr);
			}
			else if (propertyType == rttr::type::get<Vec3f>())
			{
				Vec3f* ptr = metaDataPtr->GetMemberPtr<Vec3f>(component, propName);
				ImGui::InputFloat3(propName.c_str(), (float*)ptr);
			}
			else if (propertyType.is_derived_from(rttr::type::get<SAsset>()))
			{
				SAsset* ptr = *metaDataPtr->GetMemberPtr<SAsset*>(component, propName);
				if (ptr)
				{
					ImGui::LabelText(propName.c_str(), ptr->GetFileName().c_str());
				}
				else
				{
					ImGui::LabelText(propName.c_str(), "No Asset...");
				}
				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("TESTDRAG"))
					{
						SUUID* assetID = (SUUID*)(payload->Data);
						SAsset* draggedAsset = gEngine->GetAssetManager()->GetAsset(*assetID);
						if (propertyType.get_raw_type() == rttr::type::get(*draggedAsset))
							metaDataPtr->SetMemberAsset(component, propName, draggedAsset);
					}
					ImGui::EndDragDropTarget();
				}
			}
			else if (wrappedPropertyType.is_sequential_container())
			{
				const rttr::type& containerType = (*wrappedPropertyType.get_template_arguments().begin()).get_raw_type();
				if (containerType.is_derived_from(rttr::type::get<SAsset>()))
				{
					auto* ptr = metaDataPtr->GetMemberPtr<std::vector<SAsset*>>(component, propName);
					if (ptr)
					{
						if (ptr->empty())
						{
							ImGui::LabelText(propName.c_str(), "No Asset...");
						}
						else
						{
							for (int i = 0; i < ptr->size(); i++)
							{
								ImGui::PushID(i);
								const SAsset* asset = ptr->at(i);
								if (asset)
									ImGui::LabelText(propName.c_str(), asset->GetFileName().c_str());
								else
									ImGui::LabelText(propName.c_str(), "No Asset...");

								if (ImGui::BeginDragDropTarget())
								{
									if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("TESTDRAG"))
									{
										SUUID* assetID = (SUUID*)(payload->Data);
										SAsset* draggedAsset = gEngine->GetAssetManager()->GetAsset(*assetID);
										if (draggedAsset)
										{
											const auto& assetType = rttr::type::get(*draggedAsset);
											if (assetType == containerType)
												metaDataPtr->SetMemberAssetVector(component, propName, i, draggedAsset);
										}
									}
									ImGui::EndDragDropTarget();
								}

								ImGui::PopID();
							}
						}
					}
				}
				else
				{
					ImGui::LabelText(propName.c_str(), "No Asset...");
				}

			}
		}
	}

}
