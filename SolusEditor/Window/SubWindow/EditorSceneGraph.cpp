#include "EditorSceneGraph.h"

#include "Engine/Engine.h"

#include "Render/RenderDevice.h"

#include "IMGUI/imgui.h"

#include <algorithm>

using namespace Solus;

namespace Editor
{
	void EditorSceneGraph::Initialize()
	{}

	void EditorSceneGraph::Render()
	{
		auto* renderDevice = gEngine->GetRenderDevice();
		if (ImGui::Begin("Graph"))
		{
			const auto& entities = gEngine->GetWorld()->GetEntities();
			static int selected = -1;
			for (int i = 0; i < entities.size(); i++)
			{
				auto entity = entities[i];
				auto* info = Solus::SolusObject::GetTypeInfo(entity->GetClassId());
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
