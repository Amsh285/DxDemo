#include "EditorUISystem.h"

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"


std::vector<std::type_index> EditorUISystem::GetRequiredComponents() const
{
	return { std::type_index(typeid(EditorUIComponent)) };
}

EditorUISystem::EditorUISystem(const std::shared_ptr<dsr::input::Input>& input)
	: dsr::ecs::System(std::type_index(typeid(EditorUISystem))),
	m_input(input)
{
	OnUpdate = std::bind(&EditorUISystem::Update, this, std::placeholders::_1);
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

	if (m_input->GetKeyDown(KeyCode::MouseLeft))
	{
		std::vector<Entity> cameras = context.FindEntitiesByTag("Camera");

		if (cameras.size() < 1)
			return;

		std::shared_ptr<TransformComponent> cameraTransform = context.GetComponentFrom<TransformComponent>(cameras[0]);
		std::shared_ptr<ViewProjectionComponent> viewProjection = context.GetComponentFrom<ViewProjectionComponent>(cameras[0]);

		XMMATRIX projectionMatrix = viewProjection->GetProjectionMatrix();
		XMVECTOR projectionDeterminant = XMMatrixDeterminant(projectionMatrix);
		XMMATRIX inverseProjectionMatrix = XMMatrixInverse(&projectionDeterminant, projectionMatrix);

		XMMATRIX viewMatrix = viewProjection->GetViewMatrix();
		XMVECTOR viewDeterminant = XMMatrixDeterminant(viewMatrix);
		XMMATRIX inverseViewMatrix = XMMatrixInverse(&viewDeterminant, viewMatrix);

		//XMMATRIX toWorldSpace = XMMatrixMultiply(inverseProjectionMatrix, inverseViewMatrix);

		MousePosition position = m_input->GetMouse()->GetCurrentClientAreaPosition();

		std::shared_ptr<dsr::inputdevices::Screen> screen = m_input->GetScreen();
		float resx = 1.0f / screen->GetClientWidth();
		float resy = 1.0f / screen->GetClientHeight();

		float x = resx * position.X;
		float y = resy * position.Y;

		XMVECTOR mClipSpace = XMVectorSet(2.0f * x - 1.0f, 1.0f - 2.0f * y, 1.0f, 1.0f);
		XMVECTOR mWorldSpace = XMVector4Transform(mClipSpace, inverseProjectionMatrix);

		// memo use this vector and apply inverse of model matrix for each geometry to test
		// this will translate mWorldspace to the local space of the geometry
		// you can calculate intersection there and dont net to transform geomtry to worldspace
		mWorldSpace = XMVector4Transform(mClipSpace, inverseViewMatrix);

		XMFLOAT4 test;
		XMStoreFloat4(&test, mWorldSpace);


		// the general direction can be calculated like this but for intersection calculation it is better
		// to use mWorldSpace and apply the inverse model matrix to it like described above.
		// Important: keep the raydirection in the Worldspace relative to the camera, do not use raydirection!
		XMVECTOR raydirection = XMVector4Normalize(XMVectorSubtract(mWorldSpace, cameraTransform->GetPosition()));
		XMFLOAT4 test2;
		XMStoreFloat4(&test2, raydirection);

		std::cout << "x: " << position.X << " y: " << position.Y << std::endl;
	}
}
