#include "dsrpch.h"
#include "AStarStaticMeshPathfinder.h"

namespace dsr
{
	namespace data
	{
		namespace pathfinding
		{
			AStarStaticMeshPathfinder::AStarStaticMeshPathfinder(const StaticMesh<Vertex3F>& sourceMesh)
				: m_vertexBuffer(sourceMesh.GetVertexBuffer()), m_adjacencyList(sourceMesh.GetAdjacencyList())
			{
			}

			namespace
			{
				struct node
				{
					std::shared_ptr<node> prev;
					uint32_t vertexIndex;

					float g, h, f;

					bool operator<(const node& other) const
					{
						return f > other.f;
					}

					node(const uint32_t& vertexIndex, const float& g, const float& h)
						: vertexIndex(vertexIndex), g(g), h(h)
					{
						f = g + h;
					}

					node(const uint32_t& vertexIndex, const float& g, const float& h, const float& f)
						: vertexIndex(vertexIndex), g(g), h(h), f(f)
					{
					}

					node(const uint32_t& vertexIndex, const float& g, const float& h, const float& f, const std::shared_ptr<node>& prev)
						: vertexIndex(vertexIndex), g(g), h(h), f(f), prev(prev)
					{
					}
				};

				struct CompareSharedPtr
				{
					bool operator()(const std::shared_ptr<node>& lhs, const std::shared_ptr<node>& rhs) const
					{
						// Compare based on f() values of nodes
						return lhs->f > rhs->f; // '>' for min-heap, '<' for max-heap
					}
				};

				typedef std::priority_queue<std::shared_ptr<node>, std::vector<std::shared_ptr<node>>, CompareSharedPtr> node_priority_queue;
			}

			std::vector<uint32_t> AStarStaticMeshPathfinder::SearchSequential(const uint32_t& startIndex, const uint32_t& goalIndex)
			{
				using namespace DirectX;

				//std::vector<size_t> openListCounts;

				if (startIndex == goalIndex)
					return std::vector<uint32_t>();

				const XMVECTOR goalPosition = XMLoadFloat3(&m_vertexBuffer[goalIndex].Position);

				node_priority_queue openList;
				openList.push(std::make_shared<node>(startIndex, 0.0f, 0.0f));

				ska::flat_hash_map<uint32_t, float> openListSearch, closeList;
				openListSearch[startIndex] = 0.0f;

				unsigned int nummIterartions = 0;

				while (!openList.empty())
				{
					std::shared_ptr<node> q = openList.top();
					openList.pop();

					// besser arrays
					const std::vector<uint32_t>& adjacentIndicies = m_adjacencyList[q->vertexIndex];

					for (const uint32_t& adjacentIndex : adjacentIndicies)
					{
						++nummIterartions;

						if (adjacentIndex == goalIndex)
						{
							std::vector<uint32_t> path;
							path.push_back(adjacentIndex);
							path.push_back(q->vertexIndex);
							std::shared_ptr<node> next = q->prev;

							while (next)
							{
								path.push_back(next->vertexIndex);
								next = next->prev;
							}

							std::reverse(path.begin(), path.end());
							return path;

							// consider break;
						}

						const XMVECTOR qPosition = XMLoadFloat3(&m_vertexBuffer[q->vertexIndex].Position);
						const XMVECTOR adjacentPosition = XMLoadFloat3(&m_vertexBuffer[adjacentIndex].Position);
						XMVECTOR deltaQ = XMVectorSubtract(qPosition, adjacentPosition);
						XMVECTOR deltaGoal = XMVectorSubtract(goalPosition, adjacentPosition);

						float g = q->g + XMVectorGetX(XMVector3Dot(deltaQ, deltaQ));
						float h = XMVectorGetX(XMVector3Dot(deltaGoal, deltaGoal));
						float f = g + h;

						if (openListSearch.find(adjacentIndex) != openListSearch.end() && openListSearch[adjacentIndex] < f)
							continue;

						if (closeList.find(adjacentIndex) != closeList.end() && closeList[adjacentIndex] < f)
							continue;

						openListSearch[adjacentIndex] = f;
						std::shared_ptr<node> adjacentNode = std::make_shared<node>(adjacentIndex, g, h, f);
						adjacentNode->prev = q;
						openList.push(adjacentNode);
					}

					//openListCounts.push_back(openList.size());

					closeList[q->vertexIndex] = q->f;
				}

				return std::vector<uint32_t>();
			}


		}
	}
}
