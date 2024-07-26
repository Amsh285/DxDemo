#pragma once

#include "Data/Vertex.h"

#include "Data/Structures/StaticMesh.h"

namespace dsr
{
	namespace data
	{
		namespace pathfinding
		{
			namespace heuristics
			{
				struct EucledianDistance
				{
					float operator()(const DirectX::XMVECTOR& current, const DirectX::XMVECTOR& finish) const
					{
						return DirectX::XMVectorGetX(DirectX::XMVector3Length(DirectX::XMVectorSubtract(current, finish)));
					}
				};
			}

			class AStarPathfinder final
			{
			public:
				double GetAverrageBranchingFactor() const;
				const std::unordered_map<uint32_t, std::vector<uint32_t>>& GetAdjacencyList() const { return m_adjacencyList; }

				void SetGraph(
					const std::vector<Vertex3F>& vertexBuffer,
					const std::unordered_map<uint32_t, std::vector<uint32_t>>& adjacencyList
				);

				AStarPathfinder();
				AStarPathfinder(
					const std::vector<Vertex3F>& vertexBuffer,
					const std::unordered_map<uint32_t, std::vector<uint32_t>>& adjacencyList
				);

				template<class THeuristic>
				std::vector<uint32_t> Search(const uint32_t& startIndex, const uint32_t& goalIndex);

				template<class THeuristic>
				std::vector<uint32_t> Search(const uint32_t& startIndex, const uint32_t& goalIndex, THeuristic heuristic);
				float GetLegnth(const std::vector<uint32_t>& path) const;
			private:
				struct NodeHistoryEntry
				{
					uint32_t parentId;
					uint32_t vertexBufferIndex;

					NodeHistoryEntry()
						: parentId(0), vertexBufferIndex(std::numeric_limits<uint32_t>::max())
					{
					}

					NodeHistoryEntry(const uint32_t& parentId, const uint32_t& vertexBufferIndex)
						: parentId(parentId), vertexBufferIndex(vertexBufferIndex)
					{
					}
				};

				struct Node
				{
					uint32_t id;
					uint32_t vertexIndex;

					float g, h, f;

					Node(const uint32_t& nodeId, const uint32_t& vertexIndex, const float& g, const float& h, const float& f)
						: id(nodeId), vertexIndex(vertexIndex), g(g), h(h), f(f)
					{
					}

					bool operator<(const Node& other) const
					{
						return f > other.f;
					}

					bool operator()(const Node& left, const Node& right) const
					{
						return left.f > right.f;
					}
				};

				typedef std::priority_queue<Node, std::vector<Node>> node_priority_queue;

				std::vector<Vertex3F> m_vertexBuffer;
				std::unordered_map<uint32_t, std::vector<uint32_t>> m_adjacencyList;
			};

			template<class THeuristic>
			inline std::vector<uint32_t> AStarPathfinder::Search(const uint32_t& startIndex, const uint32_t& goalIndex)
			{
				return Search(startIndex, goalIndex, THeuristic());
			}

			template<class THeuristic>
			inline std::vector<uint32_t> AStarPathfinder::Search(const uint32_t& startIndex, const uint32_t& goalIndex, THeuristic heuristic)
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
		}
	}
}
