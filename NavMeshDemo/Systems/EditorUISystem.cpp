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
	const std::shared_ptr<dsr::scene::SceneManager>& sceneManager,
	const std::shared_ptr<dsr::input::Input>& input,
	const std::shared_ptr<RampScene>& rampScene)
	: dsr::ecs::System(std::type_index(typeid(EditorUISystem))),
	m_sceneManager(sceneManager), m_input(input), m_rampScene(rampScene), m_sceneSelectedIdx(0)
{
	OnUpdate = std::bind(&EditorUISystem::Update, this, std::placeholders::_1);
	OnStart = std::bind(&EditorUISystem::Start, this, std::placeholders::_1);
}

void EditorUISystem::Start(const dsr::ecs::EngineStartupContext& context)
{
	using namespace dsr::scene;

	std::vector<std::shared_ptr<Scene>> scenes = m_sceneManager->GetScenes();

	for (size_t i = 0; i < scenes.size(); ++i)
	{
		std::shared_ptr<Scene> scene = scenes[i];
		SceneViewData viewData;
		viewData.SceneName = scene->GetName();
		viewData.SceneId = scene->GetSceneId();
		m_sceneViewData.push_back(viewData);

		if (scene->GetName() == "Ramp")
			m_sceneSelectedIdx = i;
	}
}

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
		for (size_t i = 0; i < m_sceneViewData.size(); i++)
		{
			//m_sceneViewData[i].IsSelected = itemCurrentIndex == i;
			const bool isSelected = m_sceneSelectedIdx == i;

			if (ImGui::Selectable(m_sceneViewData[i].SceneName.c_str(), isSelected))
			{
				if (m_sceneSelectedIdx != i)
					m_sceneManager->SetActiveScene(m_sceneViewData[i].SceneId);

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
		//Todo: remove if later!
		if (m_sceneViewData[m_sceneSelectedIdx].SceneName == "Ramp")
		{
			EditorScreenClickEvent screenClickEvent(
				m_input->GetMouse()->GetCurrentClientAreaPosition(),
				*m_input->GetScreen(),
				(PathSelectType)uiData->PathSelectMode);

			m_rampScene->OnScreenClick(screenClickEvent);
		}
	}
}
