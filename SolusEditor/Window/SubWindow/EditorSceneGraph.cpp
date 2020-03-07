#include "EditorSceneGraph.h"

#include "Engine/Engine.h"
#include "Object/World.h"

#include "Render/RenderDevice.h"

#include "Object/TestEntity.h"

#include "IMGUI/imgui.h"

#include <algorithm>

using namespace Solus;

namespace Editor
{
	void EditorSceneGraph::Initialize()
	{
		windowFlags = ImGuiWindowFlags_NoCollapse;
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
				auto* info = Solus::ClassMetaData::Get(entity->GetClassId());
				char name[128];
				sprintf_s(name, "%s", info->name);
				ImGui::PushID((int)entity->GetId());
				if (ImGui::Selectable(name, selected == i))
				{
					selected = i;
					selectedEntity = const_cast<Solus::Entity*>(entity);
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
