#pragma once

#include "Data/Manipulation/StaticMeshExtensions.h"

#include "ErrorHandling/dsr_error.h"
#include "ErrorHandling/NotFoundError.h"

#include "VertexIndexSearchResult.h"

namespace dsr
{
	namespace data
	{
		namespace pathfinding
		{
			template<class TVertex>
			std::variant<VertexIndexSearchResult, NotFoundError> SearchNearestVertexIndices(
				const DirectX::XMVECTOR& start,
				const DirectX::XMVECTOR& finish,
				const StaticMesh<TVertex>& mesh
			)
			{
				using namespace dsr::data::manipulation;

				// Find the triangles intersected by the start and finish points
				std::optional<StaticMeshTriangle> startTriangle = FindIntersectionTriangle(start, mesh);
				std::optional<StaticMeshTriangle> finishTriangle = FindIntersectionTriangle(finish, mesh);

				/*possible fix:
				1.) check if the vertex start/finish intersects with an edge
				2.) if it intersects with the edge search all triangles containing that edge with an epsilon tolerance
				3.) use these triangles to compute the shortest possible distance1

				Also Todo: Test Subdivision the indexbuffer is not optimal for barycentric indices are only reused in the
				triangle to be subdivided they should be reused for any vertix matching the index
				*/


				/*std::vector<StaticMeshTriangle> startTriangles = FindIntersectionTriangles(start, mesh);
				std::vector<StaticMeshTriangle> finishTriangles = FindIntersectionTriangles(finish, mesh);*/

				// If either triangle is not found, return an error
				if (!startTriangle.has_value() || !finishTriangle.has_value()) {
					return NotFoundError("Intersection Triangles not found.");
				}

				// If the start and finish points intersect the same triangle
				if (startTriangle.value().Equal(finishTriangle.value())) {
					return VertexIndexSearchResult(0, 0, true);
				}

				// If the start and finish points intersect different triangles, find the closest vertices
				const StaticMeshTriangle t0 = startTriangle.value();
				const StaticMeshTriangle t1 = finishTriangle.value();

				std::pair<uint32_t, uint32_t> closest = FindClosestVertices(
					t0.V0, t0.Index0,
					t0.V1, t0.Index1,
					t0.V2, t0.Index2,
					t1.V0, t1.Index0,
					t1.V1, t1.Index1,
					t1.V2, t1.Index2
				);

				return VertexIndexSearchResult(closest.first, closest.second, false);
			}
		}
	}
}
