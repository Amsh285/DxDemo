#pragma once

namespace dsr
{
	namespace data
	{
		template<class TVertex>
		class StaticMesh
		{
		public:
			const std::vector<TVertex>& GetVertexBuffer() const { return m_vertexBuffer; }
			void SetVertexBuffer(const std::vector<TVertex>& vertexBuffer) { m_vertexBuffer = vertexBuffer; }

			const std::vector<uint32_t>& GetIndexBuffer() const { return m_indexBuffer; }
			void SetIndexBuffer(const std::vector<uint32_t>& indexBuffer) { m_indexBuffer = indexBuffer; }
		private:
			std::vector<TVertex> m_vertexBuffer;
			std::vector<uint32_t> m_indexBuffer;
		};
	}
}
