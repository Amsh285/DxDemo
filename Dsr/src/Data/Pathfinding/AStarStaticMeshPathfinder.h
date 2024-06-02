#pragma once

#include "Data/Vertex.h"

#include "Data/Structures/StaticMesh.h"

namespace dsr
{
	namespace data
	{
		namespace pathfinding
		{
			class AStarStaticMeshPathfinder
			{
			public:
				AStarStaticMeshPathfinder(const StaticMesh<Vertex3F>& sourceMesh);

				std::vector<uint32_t> SearchSequential(const uint32_t& startIndex, const uint32_t& goalIndex);
			private:
				std::vector<Vertex3F> m_vertexBuffer;
				std::unordered_map<uint32_t, std::vector<uint32_t>> m_adjacencyList;
			};
		}
	}
}
