#pragma once

#include "Data/Vertex.h"

#include "Data/Structures/StaticMesh.h"

namespace dsr
{
	namespace data
	{
		namespace pathfinding
		{
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

				std::vector<uint32_t> Search(const uint32_t& startIndex, const uint32_t& goalIndex);
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
		}
	}
}
