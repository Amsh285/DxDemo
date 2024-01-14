#pragma once

#include "DirectX/Direct3dVertexBufferf.h"
#include "DirectX/Rendering/Data/VertexGroup.h"

namespace dsr
{
	namespace ecs
	{
		class StaticMeshComponent
		{
		public:
			std::vector<std::shared_ptr<directX::rendering::VertexGroup>> GetVertexGroups() const { return m_vertexGroups; }
			void SetVertexGroups(const std::vector<std::shared_ptr<directX::rendering::VertexGroup>>& vertexGroups) { m_vertexGroups = vertexGroups; }

			directX::Direct3dVertexBufferf GetVertexBuffer()const { return m_vertexBuffer; }
			void SetVertexBuffer(const directX::Direct3dVertexBufferf& vertexBuffer) { m_vertexBuffer = vertexBuffer; }
		private:

			// Remove sharedptr later on. Need to refactor ModelConfiguration and old renderer depends on it.
			// Need the old renderer as fallback.
			// For now let it stay here until the new renderer works. then maybe remove it.
			std::vector<std::shared_ptr<directX::rendering::VertexGroup>> m_vertexGroups;
			directX::Direct3dVertexBufferf m_vertexBuffer;
		};
	}
}
