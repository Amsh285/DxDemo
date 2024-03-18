#pragma once

#include "Data/Vertex.h"

#include "Data/Structures/StaticMesh.h"

namespace dsr
{
	namespace data
	{
		namespace pathfinding
		{
			struct NodeIB
			{
				uint32_t Index;
				float G, H;

				float F() const { return G + H; }

				NodeIB()
					: Index(std::numeric_limits<uint32_t>::max()), G(0.0f), H(0.0f)
				{}

				NodeIB(const uint32_t& index)
					: Index(index), G(0.0f), H(0.0f)
				{
				}

				NodeIB(const uint32_t& index, const float& g, const float& h)
					: Index(index), G(g), H(h)
				{
				}
			};

			struct NodeIBCostComparer
			{
				bool operator()(const NodeIB& lhs, const NodeIB& rhs) const
				{
					return lhs.F() < rhs.F();
				}
			};

			class AStarStaticMeshPathfinder
			{
			public:
				AStarStaticMeshPathfinder(const StaticMesh<Vertex3F>& sourceMesh);

				std::vector<uint32_t> SearchIndexPath(const uint32_t& startIndex, const uint32_t& goalIndex);
				std::vector<uint32_t> SearchIndexPathImp(const uint32_t& startIndex, const uint32_t& goalIndex);
			private:
				std::vector<Vertex3F> m_vertexBuffer;
				std::unordered_map<uint32_t, std::set<uint32_t>> m_adjacencyList;
			};
		}
	}
}
