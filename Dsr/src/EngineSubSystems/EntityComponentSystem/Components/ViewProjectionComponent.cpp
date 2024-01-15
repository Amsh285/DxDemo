#include "dsrpch.h"
#include "ViewProjectionComponent.h"

namespace dsr
{
	namespace ecs
	{
		ViewProjectionComponent::ViewProjectionComponent()
		{
			m_viewMatrix = DirectX::XMMatrixIdentity();
			m_projectionMatrix = DirectX::XMMatrixIdentity();
		}
	}
}
