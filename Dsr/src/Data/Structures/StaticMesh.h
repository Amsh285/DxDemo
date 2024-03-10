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
				////each vertex from the current triangle is adjacent to each other
				//adjacencyList[m_indexBuffer[i]].insert(m_indexBuffer[i + 1]);
				//adjacencyList[m_indexBuffer[i]].insert(m_indexBuffer[i + 2]);

				//adjacencyList[m_indexBuffer[i + 1]].insert(m_indexBuffer[i]);
				//adjacencyList[m_indexBuffer[i + 1]].insert(m_indexBuffer[i + 2]);

				//adjacencyList[m_indexBuffer[i + 2]].insert(m_indexBuffer[i]);
				//adjacencyList[m_indexBuffer[i + 2]].insert(m_indexBuffer[i + 1]);

				//// look for triangles after the current triangle
				//for (size_t j = i + 3; j < m_indexBuffer.size(); j += 3)
				//{
				//	InsertAdjacentIndices(i, j, adjacencyList);
				//	InsertAdjacentIndices(i + 1, j, adjacencyList);
				//	InsertAdjacentIndices(i + 2, j, adjacencyList);
				//}

				//// look for triangles before the current triangle
				//for (size_t j = 0; j < i; j += 3)
				//{
				//	InsertAdjacentIndices(i, j, adjacencyList);
				//	InsertAdjacentIndices(i + 1, j, adjacencyList);
				//	InsertAdjacentIndices(i + 2, j, adjacencyList);
				//}

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
			if (m_indexBuffer[currentTrianlgeIndex] == m_indexBuffer[nextTriangleIndex])
			{
				adjacencyList[m_indexBuffer[currentTrianlgeIndex]].insert(m_indexBuffer[nextTriangleIndex + 1]);
				adjacencyList[m_indexBuffer[currentTrianlgeIndex]].insert(m_indexBuffer[nextTriangleIndex + 2]);
			}
			else if (m_indexBuffer[currentTrianlgeIndex] == m_indexBuffer[nextTriangleIndex + 1])
			{
				adjacencyList[m_indexBuffer[currentTrianlgeIndex]].insert(m_indexBuffer[nextTriangleIndex]);
				adjacencyList[m_indexBuffer[currentTrianlgeIndex]].insert(m_indexBuffer[nextTriangleIndex + 2]);
			}
			else if (m_indexBuffer[currentTrianlgeIndex] == m_indexBuffer[nextTriangleIndex + 2])
			{
				adjacencyList[m_indexBuffer[currentTrianlgeIndex]].insert(m_indexBuffer[nextTriangleIndex]);
				adjacencyList[m_indexBuffer[currentTrianlgeIndex]].insert(m_indexBuffer[nextTriangleIndex + 1]);
			}
		}
	}
}
