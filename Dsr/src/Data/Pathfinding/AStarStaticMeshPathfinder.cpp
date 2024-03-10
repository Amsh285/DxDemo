#include "dsrpch.h"
#include "AStarStaticMeshPathfinder.h"

namespace dsr
{
	namespace data
	{
		namespace pathfinding
		{
			AStarStaticMeshPathfinder::AStarStaticMeshPathfinder(const StaticMesh<Vertex3FP2FTx3FN>& sourceMesh)
				: m_adjacencyList(sourceMesh.GetAdjacencyList())
			{
				for (const Vertex3FP2FTx3FN& v : sourceMesh.GetVertexBuffer())
				{
					m_vertexBuffer.push_back(Vertex3F(v.Position));
				}
			}

			AStarStaticMeshPathfinder::AStarStaticMeshPathfinder(const StaticMesh<Vertex3F>& sourceMesh)
				: m_vertexBuffer(sourceMesh.GetVertexBuffer()), m_adjacencyList(sourceMesh.GetAdjacencyList())
			{
			}

			std::vector<uint32_t> AStarStaticMeshPathfinder::SearchIndexPath(const uint32_t& startIndex, const uint32_t& goalIndex)
			{
				using namespace DirectX;

				if (startIndex == goalIndex)
					return std::vector<uint32_t>();

				std::unordered_map<uint32_t, std::vector<NodeIB>> pathMap;

				std::vector<NodeIB> openList, closeList;
				openList.push_back(NodeIB(startIndex));

				const XMVECTOR goalPosition = XMLoadFloat3(&m_vertexBuffer[goalIndex].Position);

				while (!openList.empty())
				{
					std::sort(
						openList.begin(),
						openList.end(),
						[](const NodeIB& lhs, const NodeIB& rhs) { return lhs.F() < rhs.F(); }
					);

					auto it = openList.begin();
					NodeIB q = *it;
					openList.erase(it);

					std::set<uint32_t> adjacentIndicies = m_adjacencyList[q.Index];

					for (const uint32_t& adjacentIndex : adjacentIndicies)
					{
						std::vector<NodeIB> parentPath = pathMap[q.Index];

						if (adjacentIndex == goalIndex)
						{
							std::vector<uint32_t> path;

							for (const NodeIB& n : parentPath)
								path.push_back(n.Index);

							path.push_back(q.Index);
							path.push_back(adjacentIndex);

							return path;
						}

						const XMVECTOR qPosition = XMLoadFloat3(&m_vertexBuffer[q.Index].Position);
						const XMVECTOR adjacentPosition = XMLoadFloat3(&m_vertexBuffer[adjacentIndex].Position);
						XMVECTOR deltaQ = XMVectorSubtract(qPosition, adjacentPosition);
						XMVECTOR deltaGoal = XMVectorSubtract(goalPosition, adjacentPosition);

						float parentG = parentPath.empty() ? 0.0f : parentPath.back().G;
						float g = parentG + XMVectorGetX(XMVector3Dot(deltaQ, deltaQ));
						float h = XMVectorGetX(XMVector3Dot(deltaGoal, deltaGoal));

						NodeIB adjacentnode(adjacentIndex, g, h);

						auto itOpenList = std::find_if(
							openList.begin(),
							openList.end(),
							[&adjacentnode](const NodeIB& current) {
								return current.Index == adjacentnode.Index && current.F() < adjacentnode.F();
							}
						);

						auto itCloseList = std::find_if(
							closeList.begin(),
							closeList.end(),
							[&adjacentnode](const NodeIB& current) {
								return current.Index == adjacentnode.Index && current.F() < adjacentnode.F();
							}
						);

						if (itOpenList == openList.end() && itCloseList == closeList.end())
						{
							std::vector<NodeIB> parentPath = pathMap[q.Index];
							std::vector<NodeIB> path(parentPath.begin(), parentPath.end());
							path.push_back(q);
							pathMap[adjacentnode.Index] = path;
							openList.push_back(adjacentnode);
						}
					}

					closeList.push_back(q);
				}

				return std::vector<uint32_t>();
			}
		}
	}
}
