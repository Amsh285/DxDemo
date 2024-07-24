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
				std::vector<Vertex3F> m_vertexBuffer;
				std::unordered_map<uint32_t, std::vector<uint32_t>> m_adjacencyList;
			};
		}
	}
}
