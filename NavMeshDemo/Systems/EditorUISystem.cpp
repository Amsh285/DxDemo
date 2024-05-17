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
	const std::shared_ptr<dsr::scenesystem::SceneManager>& sceneManager,
	const std::shared_ptr<dsr::input::Input>& input)
	: dsr::ecs::System(std::type_index(typeid(EditorUISystem))),
	m_sceneManager(sceneManager), m_input(input), m_sceneSelectedIdx(0)
{
	OnUpdate = std::bind(&EditorUISystem::Update, this, std::placeholders::_1);
	OnStart = std::bind(&EditorUISystem::Start, this, std::placeholders::_1);
}

void EditorUISystem::Start(const dsr::ecs::EngineStartupContext& context)
{
	using namespace dsr::scenesystem;

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

	//sweet
	ImGui::Begin("Raycasting", nullptr);
	ImGui::RadioButton("Startpoint", &uiData->RaycastMode, 0);
	ImGui::RadioButton("Endpoint", &uiData->RaycastMode, 1);
	ImGui::NewLine();
	ImGui::Checkbox("Show Colliders", &uiData->ShowColliders);
	ImGui::End();

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
	}

	ImGui::End();

	if (m_input->GetKeyDown(KeyCode::MouseLeft))
	{
		std::vector<Entity> cameras = context.FindEntitiesByTag("Camera");

		if (cameras.size() < 1)
			return;

		std::shared_ptr<TransformComponent> cameraTransform = context.GetComponentFrom<TransformComponent>(cameras[0]);
		std::shared_ptr<ViewProjectionComponent> viewProjection = context.GetComponentFrom<ViewProjectionComponent>(cameras[0]);

		XMMATRIX projectionMatrix = viewProjection->GetProjectionMatrix();
		XMMATRIX viewMatrix = viewProjection->GetViewMatrix();

		MousePosition position = m_input->GetMouse()->GetCurrentClientAreaPosition();
		std::shared_ptr<dsr::inputdevices::Screen> screen = m_input->GetScreen();

		
		XMVECTOR rayDirectionWorldSpace = dsr::ScreenToWorld(
			 position.X, position.Y,
			 screen->GetClientWidth(), screen->GetClientHeight(),
			 projectionMatrix, viewMatrix
		);

		// the general direction can be calculated like this but for intersection calculation it is better
		// to use mWorldSpace and apply the inverse model matrix to it like described above.
		// Important: keep the raydirection in the Worldspace relative to the camera, do not use raydirection!
		XMVECTOR raydirection = XMVector4Normalize(XMVectorSubtract(rayDirectionWorldSpace, cameraTransform->GetPosition()));
		XMFLOAT4 test2;
		XMStoreFloat4(&test2, raydirection);

		//std::cout << "x: " << position.X << " y: " << position.Y << std::endl;
	}
}
