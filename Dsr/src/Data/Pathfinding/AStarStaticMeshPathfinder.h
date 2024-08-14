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
				double GetAvgConnectivity() const { return m_pathfinder.GetAvgConnectivity(); }
				const std::unordered_map<uint32_t, std::vector<uint32_t>>& GetAdjacencyList() const { return m_pathfinder.GetAdjacencyList(); }
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

				template<class THeuristic>
				std::vector<uint32_t> Search(const uint32_t& startIndex, const uint32_t& goalIndex);
				
				float GetLengthCoTriangular(
					const DirectX::XMVECTOR& start,
					const DirectX::XMVECTOR& finish
				) const;

				float GetLengthConcurrent(
					const DirectX::XMVECTOR& start,
					const DirectX::XMVECTOR& finish,
					const uint32_t& connectionIndex
				) const;

				float GetLength(const std::vector<uint32_t>& path);

				float GetTotalLength(
					const DirectX::XMVECTOR& start,
					const DirectX::XMVECTOR& finish,
					const std::vector<uint32_t>& path
				) const;

			private:
				AStarPathfinder m_pathfinder;
				StaticMesh<Vertex3F> m_navMesh;
			};

			template<class THeuristic>
			inline std::vector<uint32_t> AStarStaticMeshPathfinder::Search(const uint32_t& startIndex, const uint32_t& goalIndex)
			{
				return m_pathfinder.Search<THeuristic>(startIndex, goalIndex);
			}
		}
	}
}
