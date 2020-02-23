#include "EditorSceneGraph.h"

#include "Engine/Engine.h"
#include "Object/World.h"

#include "Render/RenderDevice.h"

#include "IMGUI/imgui.h"

#include <algorithm>

using namespace Solus;

namespace Editor
{
	void EditorSceneGraph::Initialize()
	{
		windowFlags = /*ImGuiWindowFlags_NoCollapse |*/ ImGuiWindowFlags_NoResize;
	}

	void EditorSceneGraph::Render()
	{
		auto size = gEngine->GetWindow()->GetWindowSize();
		ImGui::SetNextWindowPos(ImVec2(size.x * WINDOW_GRAPH_POS_X, size.y * WINDOW_GRAPH_POS_Y));
		ImGui::SetNextWindowSize(ImVec2(size.x * WINDOW_GRAPH_SIZE_X, size.y * WINDOW_GRAPH_SIZE_Y));
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
				ImGui::PushID(entity->GetId());
				if (ImGui::Selectable(name, selected == i))
				{
					selected = i;
					selectedEntity = const_cast<Solus::Entity*>(entity);
				}
				ImGui::PopID();
			}
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
