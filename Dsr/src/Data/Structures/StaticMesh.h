#pragma once

#include "Data/WindingOrder.h"

#include "Infrastructure/Comparers.h"

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

			std::unordered_map<uint32_t, std::set<uint32_t>> GetAdjacencyList() const;

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
				std::unordered_map<uint32_t, std::set<uint32_t>>& adjacencyList) const;

			std::vector<TVertex> m_vertexBuffer;
			std::vector<uint32_t> m_indexBuffer;

			WindingOrder m_order;
		};

		template<class TVertex>
		inline std::unordered_map<uint32_t, std::set<uint32_t>> StaticMesh<TVertex>::GetAdjacencyList() const
		{
			std::unordered_map<uint32_t, std::set<uint32_t>> adjacencyList;

			for (size_t i = 0; i < m_indexBuffer.size(); i += 3)
			{
				for (size_t j = 0; j < m_indexBuffer.size(); j += 3)
				{
					InsertAdjacentIndices(i, j, adjacencyList);
					InsertAdjacentIndices(i + 1, j, adjacencyList);
					InsertAdjacentIndices(i + 2, j, adjacencyList);
				}
			}

			return adjacencyList;
		}

		template<class TVertex>
		inline void StaticMesh<TVertex>::InsertAdjacentIndices(
			const size_t& currentTrianlgeIndex,
			const size_t& nextTriangleIndex,
			std::unordered_map<uint32_t, std::set<uint32_t>>& adjacencyList
		) const
		{
			using namespace DirectX;

			static dsr::XMVectorEqualComparer<1e-6f> comparer;

			/*Note:
			* 
			* this implementation relies only on indices which is wrong because vertices can be neighbours even if they are
			* different
			* e.g.:
			* 
			* v1:
			* position: x=-2.74569201 y=8.71415329 z=1.83298802
			* normal: x=-0.00000000 y=1.00000000 z=0.00000000
			* v2:
			* position: x=-2.74569201 y=8.71415329 z=1.83298802
			* normal: x=-0.00000000 y=0.894999981 z=-0.446099997
			* 
			* Both vertices are different yet they share the same position. Computing a adjacencylist
			* these vertices have to be considered equal, even if they are technically not.
			*/

			XMVECTOR currentVertex = XMLoadFloat3(&m_vertexBuffer[m_indexBuffer[currentTrianlgeIndex]].Position);
			XMVECTOR v0 = XMLoadFloat3(&m_vertexBuffer[m_indexBuffer[nextTriangleIndex]].Position);
			XMVECTOR v1 = XMLoadFloat3(&m_vertexBuffer[m_indexBuffer[nextTriangleIndex + 1]].Position);
			XMVECTOR v2 = XMLoadFloat3(&m_vertexBuffer[m_indexBuffer[nextTriangleIndex + 2]].Position);

			if (comparer.operator()(currentVertex, v0))
			{
				adjacencyList[m_indexBuffer[currentTrianlgeIndex]].insert(m_indexBuffer[nextTriangleIndex + 1]);
				adjacencyList[m_indexBuffer[currentTrianlgeIndex]].insert(m_indexBuffer[nextTriangleIndex + 2]);
			}
			else if (comparer.operator()(currentVertex, v1))
			{
				adjacencyList[m_indexBuffer[currentTrianlgeIndex]].insert(m_indexBuffer[nextTriangleIndex]);
				adjacencyList[m_indexBuffer[currentTrianlgeIndex]].insert(m_indexBuffer[nextTriangleIndex + 2]);
			}
			else if (comparer.operator()(currentVertex, v2))
			{
				adjacencyList[m_indexBuffer[currentTrianlgeIndex]].insert(m_indexBuffer[nextTriangleIndex]);
				adjacencyList[m_indexBuffer[currentTrianlgeIndex]].insert(m_indexBuffer[nextTriangleIndex + 1]);
			}

			/*if (currentIndexIB == m_indexBuffer[nextTriangleIndex])
			{
				adjacencyList[m_indexBuffer[currentTrianlgeIndex]].insert(m_indexBuffer[nextTriangleIndex + 1]);
				adjacencyList[m_indexBuffer[currentTrianlgeIndex]].insert(m_indexBuffer[nextTriangleIndex + 2]);
			}
			else if (currentIndexIB == m_indexBuffer[nextTriangleIndex + 1])
			{
				adjacencyList[m_indexBuffer[currentTrianlgeIndex]].insert(m_indexBuffer[nextTriangleIndex]);
				adjacencyList[m_indexBuffer[currentTrianlgeIndex]].insert(m_indexBuffer[nextTriangleIndex + 2]);
			}
			else if (currentIndexIB == m_indexBuffer[nextTriangleIndex + 2])
			{
				adjacencyList[m_indexBuffer[currentTrianlgeIndex]].insert(m_indexBuffer[nextTriangleIndex]);
				adjacencyList[m_indexBuffer[currentTrianlgeIndex]].insert(m_indexBuffer[nextTriangleIndex + 1]);
			}*/
		}
	}
}
