#include "EditorUISystem.h"

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

#include "Infrastructure/XMathHelper.h"

std::vector<std::type_index> EditorUISystem::GetRequiredComponents() const
{
	return { std::type_index(typeid(EditorUIComponent)) };
}

EditorUISystem::EditorUISystem(
	const std::shared_ptr<dsr::input::Input>& input,
	const std::vector<std::shared_ptr<NavMeshSimulationSceneBase>>& scenes)
	: dsr::ecs::System(typeid(EditorUISystem)),
	m_input(input), m_scenes(scenes), m_sceneSelectedIdx(0)
{
	//OnStart = std::bind(&EditorUISystem::Start, this, std::placeholders::_1);
	OnUpdate = std::bind(&EditorUISystem::Update, this, std::placeholders::_1);
}

//void EditorUISystem::Start(const dsr::ecs::EngineStartupContext& context)
//{
//}

void EditorUISystem::Update(const dsr::ecs::EngineContext& context)
{
	using namespace dsr::ecs;
	using namespace dsr::events;
	using namespace dsr::input;

	using namespace DirectX;

	std::shared_ptr<EditorUIComponent> uiData = context.GetComponent<EditorUIComponent>();

	ImGui::Begin("Editor", nullptr);

	if (ImGui::BeginListBox("Scenes"))
	{
		for (size_t i = 0; i < m_scenes.size(); i++)
		{
			//m_sceneViewData[i].IsSelected = itemCurrentIndex == i;
			const bool isSelected = m_sceneSelectedIdx == i;

			if (ImGui::Selectable(m_scenes[i]->GetSceneName().c_str(), isSelected))
			{
				if (m_sceneSelectedIdx != i)
					m_scenes[i]->SetActive();

				m_sceneSelectedIdx = i;
			}

			if (isSelected)
				ImGui::SetItemDefaultFocus();
		}

		ImGui::EndListBox();
		ImGui::NewLine();

		if (ImGui::CollapsingHeader("Debug"))
		{
			ImGui::Checkbox("Show Colliders", &uiData->ShowColliders);
		}

		if (ImGui::CollapsingHeader("Pathfinding"))
		{
			ImGui::Text("Node select:");
			ImGui::RadioButton("Start", &uiData->PathSelectMode, 0);
			ImGui::RadioButton("Finish", &uiData->PathSelectMode, 1);
		}
	}

	ImGui::End();

	if (m_input->GetKeyDown(KeyCode::MouseLeft))
	{
		EditorScreenClickEvent screenClickEvent(
			m_input->GetMouse()->GetCurrentClientAreaPosition(),
			*m_input->GetScreen(),
			(PathSelectType)uiData->PathSelectMode);

		m_scenes[m_sceneSelectedIdx]->OnScreenClick(screenClickEvent);
	}
}
