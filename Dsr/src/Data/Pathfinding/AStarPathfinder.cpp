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

			std::vector<uint32_t> AStarPathfinder::Search(const uint32_t& startIndex, const uint32_t& goalIndex)
			{
				using namespace DirectX;

				if (startIndex == goalIndex)
					return std::vector<uint32_t>();

				const XMVECTOR goalPosition = XMLoadFloat3(&m_vertexBuffer[goalIndex].Position);

				uint32_t currentId = 1;
				ska::flat_hash_map<uint32_t, NodeHistoryEntry> nodeIdHistories;

				node_priority_queue openList;
				openList.push(Node(currentId, startIndex, 0.0f, 0.0f, 0.0f));

				ska::flat_hash_map<uint32_t, float> openListSearch, closeList;

				openListSearch[startIndex] = 0.0f;
				nodeIdHistories[currentId] = NodeHistoryEntry(0, startIndex);

				while (!openList.empty())
				{
					const Node q = openList.top();
					openList.pop();

					const std::vector<uint32_t>& adjacentIndicies = m_adjacencyList[q.vertexIndex];

					for (const uint32_t& adjacentIndex : adjacentIndicies)
					{
						if (adjacentIndex == goalIndex)
						{
							std::vector<uint32_t> path;
							path.push_back(adjacentIndex);

							NodeHistoryEntry* next = &nodeIdHistories[q.id];

							while (next->parentId != 0)
							{
								path.push_back(next->vertexBufferIndex);
								next = &nodeIdHistories[next->parentId];
							}

							path.push_back(next->vertexBufferIndex);

							//std::reverse(path.begin(), path.end());
							return path;
						}

						const XMVECTOR qPosition = XMLoadFloat3(&m_vertexBuffer[q.vertexIndex].Position);
						const XMVECTOR adjacentPosition = XMLoadFloat3(&m_vertexBuffer[adjacentIndex].Position);
						XMVECTOR deltaQ = XMVectorSubtract(qPosition, adjacentPosition);
						XMVECTOR deltaGoal = XMVectorSubtract(goalPosition, adjacentPosition);

						float g = q.g + XMVectorGetX(XMVector3Length(deltaQ));
						float h = XMVectorGetX(XMVector3Length(deltaGoal));
						float f = g + h;

						if (openListSearch.find(adjacentIndex) != openListSearch.end() && openListSearch[adjacentIndex] < f)
							continue;

						if (closeList.find(adjacentIndex) != closeList.end() && closeList[adjacentIndex] < f)
							continue;

						openListSearch[adjacentIndex] = f;
						++currentId;
						nodeIdHistories[currentId] = NodeHistoryEntry(q.id, adjacentIndex);
						openList.push(Node(currentId, adjacentIndex, g, h, f));
					}

					closeList[q.vertexIndex] = q.f;
				}

				return std::vector<uint32_t>();
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
