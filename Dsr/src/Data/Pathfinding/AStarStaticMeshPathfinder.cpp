#include "dsrpch.h"
#include "AStarStaticMeshPathfinder.h"

#include "Data/Manipulation/StaticMeshExtensions.h"
#include "Data/Pathfinding/PathPreProcessor.h"

namespace dsr
{
	namespace data
	{
		namespace pathfinding
		{
			void AStarStaticMeshPathfinder::SetGraph(const StaticMesh<Vertex3FP2FTx3FN>& navMesh)
			{
				using namespace manipulation;

				m_navMesh = FilterDistinct(navMesh);
				m_pathfinder.SetGraph(m_navMesh.GetVertexBuffer(), m_navMesh.GetAdjacencyList());
			}

			void AStarStaticMeshPathfinder::SetGraph(const StaticMesh<Vertex3F>& navMesh)
			{
				m_navMesh = navMesh;
				m_pathfinder.SetGraph(m_navMesh.GetVertexBuffer(), m_navMesh.GetAdjacencyList());
			}

			AStarStaticMeshPathfinder::AStarStaticMeshPathfinder()
			{
			}

			AStarStaticMeshPathfinder::AStarStaticMeshPathfinder(const StaticMesh<Vertex3F>& navMesh)
				: m_navMesh(navMesh)
			{
				m_pathfinder.SetGraph(m_navMesh.GetVertexBuffer(), m_navMesh.GetAdjacencyList());
			}

			AStarStaticMeshPathfinder::AStarStaticMeshPathfinder(const StaticMesh<Vertex3FP2FTx3FN>& navMesh)
			{
				using namespace manipulation;

				m_navMesh = FilterDistinct(navMesh);
				m_pathfinder.SetGraph(m_navMesh.GetVertexBuffer(), m_navMesh.GetAdjacencyList());
			}

			VertexIndexSearchResult AStarStaticMeshPathfinder::SearchNearestVertexIndices(
				const DirectX::XMVECTOR& start,
				const DirectX::XMVECTOR& finish
			) const
			{
				return dsr::data::pathfinding::SearchNearestVertexIndices(start, finish, m_navMesh);
			}

			DirectX::XMVECTOR AStarStaticMeshPathfinder::GetConnectionVertex(const VertexIndexSearchResult& result) const
			{
				using namespace DirectX;

				assert(result.GetResultType() == VertexIndexSearchResultType::Concurrent);

				return XMLoadFloat3(&m_navMesh.GetVertexBuffer()[result.GetStartIndex()].Position);
			}

			std::vector<uint32_t> AStarStaticMeshPathfinder::Search(const uint32_t& startIndex, const uint32_t& goalIndex)
			{
				return m_pathfinder.Search(startIndex, goalIndex);
			}
		}
	}
}
