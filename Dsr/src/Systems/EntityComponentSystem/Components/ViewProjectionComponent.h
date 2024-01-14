#pragma once

#include "Systems/EntityComponentSystem/Component.h"

namespace dsr
{
	namespace ecs
	{
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
