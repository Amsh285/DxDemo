#include "dsrpch.h"
#include "AStarPathfinder.h"

namespace dsr
{
	namespace data
	{
		namespace pathfinding
		{
			double AStarPathfinder::GetAverrageBranchingFactor() const
			{
				double sum = 0.0;
				for (const auto& adjacency : m_adjacencyList)
					sum += adjacency.second.size();

				return sum / m_adjacencyList.size();
			}

			void AStarPathfinder::SetGraph(
				const std::vector<Vertex3F>& vertexBuffer,
				const std::unordered_map<uint32_t, std::vector<uint32_t>>& adjacencyList
			)
			{
				m_vertexBuffer = vertexBuffer;
				m_adjacencyList = adjacencyList;
			}

			AStarPathfinder::AStarPathfinder()
			{
			}

			AStarPathfinder::AStarPathfinder(
				const std::vector<Vertex3F>& vertexBuffer,
				const std::unordered_map<uint32_t, std::vector<uint32_t>>& adjacencyList
			) : m_vertexBuffer(vertexBuffer), m_adjacencyList(adjacencyList)
			{
			}

			float AStarPathfinder::GetLegnth(const std::vector<uint32_t>& path) const
			{
				using namespace DirectX;

				if(path.size() < 2)
					return 0.0f;

				float length = 0.0f;

				for (size_t i = 0; i < path.size() - 1; i++)
				{
					XMVECTOR v0 = XMLoadFloat3(&m_vertexBuffer[path[i]].Position);
					XMVECTOR v1 = XMLoadFloat3(&m_vertexBuffer[path[i + 1]].Position);

					length += XMVectorGetX(XMVector3Length(XMVectorSubtract(v0, v1)));
				}

				return length;
			}
		}
	}
}
