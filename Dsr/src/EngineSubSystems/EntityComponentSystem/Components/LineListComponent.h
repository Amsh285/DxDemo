#pragma once

#include "DirectX/Direct3dVertexBufferf.h"
#include "Data/Vertex.h"

#include "EngineSubSystems/EntityComponentSystem/Component.h"

namespace dsr
{
	namespace ecs
	{
		class LineListComponent : public Component
		{
		public:
			uint32_t GetStartVertexLocation() const { return m_startVertexLocation; }
			void SetStartVertexLocation(const uint32_t& startVertexLocation) { m_startVertexLocation = startVertexLocation; }

			uint32_t GetVertexCount() const { return m_vertexCount; }
			void SetVertexCount(const uint32_t& vertexCount) { m_vertexCount = vertexCount; }

			directX::Direct3dBuffer GetVertexBuffer()const { return m_vertexBuffer; }
			void SetVertexBuffer(const directX::Direct3dBuffer& vertexBuffer) { m_vertexBuffer = vertexBuffer; }

			directX::Direct3dShaderInputLayout GetVertexShaderInputLayout() const { return m_vertexShaderInputLayout; }
			void SetVertexShaderInputLayout(const directX::Direct3dShaderInputLayout& layout) { m_vertexShaderInputLayout = layout; }

			LineListComponent();
		private:
			uint32_t m_startVertexLocation;
			uint32_t m_vertexCount;

			directX::Direct3dBuffer m_vertexBuffer;
			directX::Direct3dShaderInputLayout m_vertexShaderInputLayout;
		};
	}
}
