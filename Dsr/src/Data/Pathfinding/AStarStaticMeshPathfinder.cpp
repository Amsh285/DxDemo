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

			//std::vector<uint32_t> AStarStaticMeshPathfinder::SearchIndexPathImp(const uint32_t& startIndex, const uint32_t& goalIndex)
			//{
			//	using namespace DirectX;

			//	if (startIndex == goalIndex)
			//		return std::vector<uint32_t>();

			//	const XMVECTOR goalPosition = XMLoadFloat3(&m_vertexBuffer[goalIndex].Position);

			//	//std::multiset<std::pair<float, std::<pair<uint32_t, uint32_t>>, std::less<float>> openListExpansion;
			//	std::multiset<std::pair<float, uint32_t>, std::less<float>> openListExpansion;
			//	openListExpansion.insert(std::pair(0.0f, startIndex));
			//	
			//	std::unordered_map<uint32_t, float> openList, closeList;
			//	openList[startIndex] = 0.0f;

			//	std::unordered_map<uint32_t, std::vector<uint32_t>> pathMap;

			//	while (openListExpansion.size() > 0)
			//	{
			//		std::pair<float, uint32_t> q = openListExpansion.extract(openListExpansion.begin()).value();
			//		const std::vector<uint32_t>& parentPath = pathMap[q.second];
			//		openList.erase(q.second);

			//		// expand q
			//		const std::set<uint32_t> adjacentIndicies = m_adjacencyList[q.second];

			//		for (const uint32_t adjacentIndex : adjacentIndicies)
			//		{
			//			// reconstruct path
			//			if (adjacentIndex == goalIndex)
			//			{
			//				std::vector<uint32_t> path;

			//				for (const uint32_t& n : parentPath)
			//					path.push_back(n);

			//				path.push_back(q.second);
			//				path.push_back(adjacentIndex);

			//				return path;
			//			}

			//			// calculate cost
			//			const XMVECTOR qPosition = XMLoadFloat3(&m_vertexBuffer[q.second].Position);
			//			const XMVECTOR adjacentPosition = XMLoadFloat3(&m_vertexBuffer[adjacentIndex].Position);
			//			XMVECTOR deltaQ = XMVectorSubtract(qPosition, adjacentPosition);
			//			XMVECTOR deltaGoal = XMVectorSubtract(goalPosition, adjacentPosition);

			//			float g = q.first + XMVectorGetX(XMVector3Dot(deltaQ, deltaQ));
			//			float h = XMVectorGetX(XMVector3Dot(deltaGoal, deltaGoal));
			//			float f = g + h;

			//			auto itOpenList = openList.find(adjacentIndex);
			//			auto itCloseList = closeList.find(adjacentIndex);

			//			if (itOpenList != openList.end() && itOpenList->first < f)
			//				continue;

			//			if (itCloseList != closeList.end() && itCloseList->first < f)
			//				continue;
			//			
			//			std::vector<uint32_t> path(parentPath.begin(), parentPath.end());
			//			path.push_back(q.second);
			//			pathMap[adjacentIndex] = path;

			//			openList[adjacentIndex] = f;
			//			openListExpansion.insert(std::pair(f, adjacentIndex));
			//		}

			//		closeList[q.second] = q.first;
			//	}

			//	return std::vector<uint32_t>();
			//}

			struct node
			{
				std::shared_ptr<node> prev;
				uint32_t vertexIndex;

				float g, h;
				float f() const { return g + h; }

				bool operator<(const node& other) const
				{
					return f() > other.f();
				}

				node(const uint32_t& vertexIndex, const float& g, const float& h)
					: vertexIndex(vertexIndex), g(g), h(h)
				{
				}
			};

			struct CompareSharedPtr
			{
				bool operator()(const std::shared_ptr<node>& lhs, const std::shared_ptr<node>& rhs) const
				{
					// Compare based on f() values of nodes
					return lhs->f() > rhs->f(); // '>' for min-heap, '<' for max-heap
				}
			};

			std::vector<uint32_t> AStarStaticMeshPathfinder::SearchIndexPathImp(const uint32_t& startIndex, const uint32_t& goalIndex)
			{
				using namespace DirectX;

				if (startIndex == goalIndex)
					return std::vector<uint32_t>();

				const XMVECTOR goalPosition = XMLoadFloat3(&m_vertexBuffer[goalIndex].Position);

				std::priority_queue<std::shared_ptr<node>, std::vector<std::shared_ptr<node>>, CompareSharedPtr> openList;
				openList.push(std::make_shared<node>(startIndex, 0.0f, 0.0f));

				std::unordered_map<uint32_t, float> openListSearch, closeList;
				openListSearch[startIndex] = 0.0f;

				while (!openList.empty())
				{
					std::shared_ptr<node> q = openList.top();
					openList.pop();

					std::set<uint32_t> adjacentIndicies = m_adjacencyList[q->vertexIndex];

					for (const uint32_t& adjacentIndex : adjacentIndicies)
					{
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
						std::shared_ptr<node> adjacentNode = std::make_shared<node>(adjacentIndex, g, h);
						adjacentNode->prev = q;
						openList.push(adjacentNode);
					}

					closeList[q->vertexIndex] = q->f();
				}

				return std::vector<uint32_t>();
			}
		}
	}
}
