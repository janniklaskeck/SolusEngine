#include "EditorSceneGraph.h"

#include "Engine/Engine.h"
#include "Object/World.h"

#include "Utility/SerializeUtil.h"

#include "Render/RenderDevice.h"

#include "Object/TestEntity.h"

#include "IMGUI/imgui.h"

#include <GLFW/glfw3.h>

#include <algorithm>

namespace Solus
{
	void EditorSceneGraph::Initialize()
	{
		windowFlags = ImGuiWindowFlags_NoCollapse;
	}

	void EditorSceneGraph::Update(float deltaTime)
	{
		if (gEngine->GetInputDevice()->IsKeyDown(GLFW_KEY_DELETE))
		{
			gEngine->GetWorld()->DestroyEntity(GetSelectedEntity());
			selectedEntity = nullptr;
		}
	}

	void EditorSceneGraph::Render()
	{
		if (ImGui::Begin("Graph", nullptr, windowFlags))
		{
			const auto& entities = gEngine->GetWorld()->GetEntities();
			static int selected = -1;
			for (int i = 0; i < entities.size(); i++)
			{
				auto entity = entities[i];

				std::string name(entity->get_type().get_name());
				ImGui::PushID(entity->GetId().ToString().c_str());
				if (ImGui::Selectable(name.c_str(), selected == i))
				{
					selected = i;
					selectedEntity = entity;
				}
				if (ImGui::BeginPopupContextItem("entityContext"))
				{
					if (ImGui::Selectable("Save As Asset"))
					{
						ArchiveStream stream("c:\\Spiele\\test.dat", ArchiveMode::WRITE);
						selectedEntity->Serialize(stream);
					}
					if (ImGui::Selectable("qwe")) gEngine->Log(LogLevel::LogDebug, "qwe");
					ImGui::EndPopup();
				}
				ImGui::PopID();
			}
		}
		if (ImGui::Button("Test"))
		{
			gEngine->GetWorld()->SpawnEntity<TestEntity>();
		}
		ImGui::End();
	}

	void EditorSceneGraph::OnFocusReceived()
	{}

	void EditorSceneGraph::OnFocusLost()
	{}

	void EditorSceneGraph::OnMinimized()
	{}

	void EditorSceneGraph::OnMaximized()
	{}

	Solus::Entity* EditorSceneGraph::GetSelectedEntity() const
	{
		return selectedEntity;
	}
}
