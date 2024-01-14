#pragma once

#include "Systems/EntityComponentSystem/Component.h"

namespace dsr
{
	namespace ecs
	{
		// I got it
		// make a system which takes a TransformComponent, CameraComponent (farZ, nearZ etc..) and a ViewProjectionComponent
		// the system will use TransformComponent and CameraComponent to calulate the matrices for ViewProjectionComponent

		class ViewProjectionComponent : public Component
		{
		public:
			DirectX::XMMATRIX GetViewMatrix() const { return m_viewMatrix; }
			void SetViewMatrix(const DirectX::XMMATRIX& viewMatrix) { m_viewMatrix = viewMatrix; }

			DirectX::XMMATRIX GetProjectionMatrix() const { return m_projectionMatrix; }
			void SetProjectionMatrix(const DirectX::XMMATRIX& projectionMatrix) { m_projectionMatrix = projectionMatrix; }

			ViewProjectionComponent();
		private:
			DirectX::XMMATRIX m_viewMatrix;
			DirectX::XMMATRIX m_projectionMatrix;
		};
	}
}
