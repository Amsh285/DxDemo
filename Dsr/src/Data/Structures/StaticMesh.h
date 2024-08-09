#pragma once

#include "Data/WindingOrder.h"

#include "Infrastructure/Comparers.h"
#include "Infrastructure/XMathHelper.h"

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

			std::unordered_map<uint32_t, std::vector<uint32_t>> GetAdjacencyList() const;

			WindingOrder GetWindingOrder() const { return m_order; }
			void SetWindingOrder(const WindingOrder& order) { m_order = order; }

			StaticMesh()
				: m_order(WindingOrder::Clockwise)
			{
			}

			StaticMesh(const WindingOrder& order)
				: m_order(order)
			{
			}
		private:
			void InsertAdjacentIndices(
				const size_t& currentTrianlgeIndex,
				const size_t& nextTriangleIndex,
				std::set<uint32_t>& neighbours) const;

			std::vector<TVertex> m_vertexBuffer;
			std::vector<uint32_t> m_indexBuffer;

			WindingOrder m_order;
		};

		template<class TVertex>
		inline std::unordered_map<uint32_t, std::vector<uint32_t>> StaticMesh<TVertex>::GetAdjacencyList() const
		{
			std::unordered_map<uint32_t, std::vector<uint32_t>> adjacencyList;

			std::mutex mutex;

#pragma omp parallel for
			for (int i = 0; i < m_indexBuffer.size(); i += 3)
			{
				std::set<uint32_t> neighboursV0, neighboursV1, neighboursV2;

				for (size_t j = 0; j < m_indexBuffer.size(); j += 3)
				{
					InsertAdjacentIndices(i, j, neighboursV0);
					InsertAdjacentIndices(i + 1, j, neighboursV1);
					InsertAdjacentIndices(i + 2, j, neighboursV2);
				}

				std::lock_guard<std::mutex> lock(mutex);
				adjacencyList[m_indexBuffer[i]] = std::vector<uint32_t>(neighboursV0.begin(), neighboursV0.end());
				adjacencyList[m_indexBuffer[i + 1]] = std::vector<uint32_t>(neighboursV1.begin(), neighboursV1.end());
				adjacencyList[m_indexBuffer[i + 2]] = std::vector<uint32_t>(neighboursV2.begin(), neighboursV2.end());
			}

			return adjacencyList;
		}

		template<class TVertex>
		inline void StaticMesh<TVertex>::InsertAdjacentIndices(
			const size_t& currentTrianlgeIndex,
			const size_t& nextTriangleIndex,
			std::set<uint32_t>& neighbours
		) const
		{
			using namespace DirectX;

			static dsr::XMVectorEqualComparer<1e-6f> comparer;

			XMVECTOR currentVertex = XMLoadFloat3(&m_vertexBuffer[m_indexBuffer[currentTrianlgeIndex]].Position);
			XMVECTOR v0 = XMLoadFloat3(&m_vertexBuffer[m_indexBuffer[nextTriangleIndex]].Position);
			XMVECTOR v1 = XMLoadFloat3(&m_vertexBuffer[m_indexBuffer[nextTriangleIndex + 1]].Position);
			XMVECTOR v2 = XMLoadFloat3(&m_vertexBuffer[m_indexBuffer[nextTriangleIndex + 2]].Position);

			if (comparer.operator()(currentVertex, v0))
			{
				neighbours.insert(m_indexBuffer[nextTriangleIndex + 1]);
				neighbours.insert(m_indexBuffer[nextTriangleIndex + 2]);
			}
			else if (comparer.operator()(currentVertex, v1))
			{
				neighbours.insert(m_indexBuffer[nextTriangleIndex]);
				neighbours.insert(m_indexBuffer[nextTriangleIndex + 2]);
			}
			else if (comparer.operator()(currentVertex, v2))
			{
				neighbours.insert(m_indexBuffer[nextTriangleIndex]);
				neighbours.insert(m_indexBuffer[nextTriangleIndex + 1]);
			}
		}
	}
}
