#pragma once

#include "Data/WindingOrder.h"

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

			WindingOrder GetWindingOrder() const { return m_order; }
			void SetWindingOrder(const WindingOrder& order) { m_order = order; }

			StaticMesh()
				: m_order(WindingOrder::Clockwise)
			{
			}

			StaticMesh(const WindingOrder& order)
				: m_order(WindingOrder::CounterClockwise)
			{
			}
		private:
			std::vector<TVertex> m_vertexBuffer;
			std::vector<uint32_t> m_indexBuffer;

			WindingOrder m_order;
		};
	}
}
