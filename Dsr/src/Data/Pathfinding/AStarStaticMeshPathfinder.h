#pragma once

#include "Data/Pathfinding/AStarPathfinder.h"
#include "Data/Pathfinding/VertexIndexSearchResult.h"
#include "Data/Structures/StaticMesh.h"

#include "ErrorHandling/NotFoundError.h"

namespace dsr
{
	namespace data
	{
		namespace pathfinding
		{
			class AStarStaticMeshPathfinder final
			{
			public:
				const StaticMesh<Vertex3F>& GetNavMesh() const { return m_navMesh; }

				void SetGraph(const StaticMesh<Vertex3FP2FTx3FN>& navMesh);
				void SetGraph(const StaticMesh<Vertex3F>& navMesh);

				AStarStaticMeshPathfinder();
				AStarStaticMeshPathfinder(const StaticMesh<Vertex3F>& navMesh);
				AStarStaticMeshPathfinder(const StaticMesh<Vertex3FP2FTx3FN>& navMesh);

				VertexIndexSearchResult SearchNearestVertexIndices(
					const DirectX::XMVECTOR& start,
					const DirectX::XMVECTOR& finish
				) const;

				DirectX::XMVECTOR GetConnectionVertex(const VertexIndexSearchResult& result) const;

				std::vector<uint32_t> Search(const uint32_t& startIndex, const uint32_t& goalIndex);

			private:
				AStarPathfinder m_pathfinder;
				StaticMesh<Vertex3F> m_navMesh;
			};
		}
	}
}
