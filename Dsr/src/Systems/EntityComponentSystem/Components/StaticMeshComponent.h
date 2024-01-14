#pragma once

#include "DirectX/Direct3dVertexBufferf.h"
#include "DirectX/Rendering/Data/VertexGroup.h"

#include "Systems/EntityComponentSystem/Component.h"

namespace dsr
{
	namespace ecs
	{
		class StaticMeshComponent : Component
		{
		public:
			std::vector<std::shared_ptr<directX::rendering::VertexGroup>> GetVertexGroups() const { return m_vertexGroups; }
			void SetVertexGroups(const std::vector<std::shared_ptr<directX::rendering::VertexGroup>>& vertexGroups) { m_vertexGroups = vertexGroups; }

			directX::Direct3dVertexBufferf GetVertexBuffer()const { return m_vertexBuffer; }
			void SetVertexBuffer(const directX::Direct3dVertexBufferf& vertexBuffer) { m_vertexBuffer = vertexBuffer; }
		private:
			std::vector<std::shared_ptr<directX::rendering::VertexGroup>> m_vertexGroups;
			directX::Direct3dVertexBufferf m_vertexBuffer;
		};
	}
}
